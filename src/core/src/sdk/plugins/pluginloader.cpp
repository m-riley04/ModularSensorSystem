#include "sdk/plugins/pluginloader.hpp"
#include <algorithm>

PluginRegistry::PluginRegistry(SettingsController& settingsController)
	: m_settingsController(settingsController)
{

}

void PluginRegistry::scan(const std::vector<std::filesystem::path>& dirs, uint32_t requiredApi) {
    m_pluginMetadata.clear();
    m_metadataByPath.clear();
    for (auto& d : dirs) if (std::filesystem::exists(d)) scanDir(d, requiredApi);
}

void PluginRegistry::scanDir(const std::filesystem::path& dir, uint32_t requiredApi) {
    for (auto& e : std::filesystem::directory_iterator(dir)) {
        if (!e.is_regular_file() || !isSharedLib(e.path()) || !looksLikePlugin(e.path())) continue;
        auto full = e.path().string();
        collectMetadata(e.path(), requiredApi);
    }
}

void PluginRegistry::collectMetadata(const std::filesystem::path& libPath, uint32_t requiredApi) {
    PluginMetadata meta; // local build
    meta.path = libPath.string();

    try {
        // Use lightweight symbol enumeration first
        boost::dll::library_info info(libPath.string()); // Ensure the path is passed as a string
        auto symbols = info.symbols();
        bool hasApi = std::find(symbols.begin(), symbols.end(), MSS_PLUGIN_API) != symbols.end();
        bool hasMake = std::find(symbols.begin(), symbols.end(), MSS_PLUGIN_API_MAKE) != symbols.end();
        bool hasDestroy = std::find(symbols.begin(), symbols.end(), MSS_PLUGIN_API_DESTROY) != symbols.end();
        meta.hasFactory = hasApi && hasMake && hasDestroy;
        if (!meta.hasFactory) {
            m_metadataByPath[meta.path] = meta; // store partial
            return; // not a valid plugin factory
        }

        // Load library lazily to query metadata without instantiation except API version
#ifdef _WIN32
        boost::dll::shared_library lib(meta.path, boost::dll::load_mode::load_with_altered_search_path | boost::dll::load_mode::rtld_lazy);
#else
        boost::dll::shared_library lib(meta.path, boost::dll::load_mode::rtld_lazy);
#endif
        if (!lib.has(MSS_PLUGIN_API)) { m_metadataByPath[meta.path] = meta; return; }
        auto apiFn = lib.get<mss_av_t*>(MSS_PLUGIN_API);
        meta.apiVersion = apiFn();
        if (meta.apiVersion != requiredApi) { m_metadataByPath[meta.path] = meta; return; }

        // Instantiate temporarily to harvest metadata (fallback approach)
        // (Could extend with dedicated metadata symbol later to avoid instantiation.)
        if (lib.has(MSS_PLUGIN_API_MAKE) && lib.has(MSS_PLUGIN_API_DESTROY)) {
            auto makeFn = lib.get<mss_make_t*>(MSS_PLUGIN_API_MAKE);
            auto destroyFn = lib.get<mss_destroy_t*>(MSS_PLUGIN_API_DESTROY);
            IPlugin* temp = nullptr;
            try { temp = makeFn(); } catch (...) { temp = nullptr; }
            if (temp) {
                meta.name = temp->name();
                meta.version = temp->version();
                meta.author = temp->author();
                meta.description = temp->description();
                meta.type = temp->type();
                meta.isCore = temp->isCore();
                // destroy temp instance
                try { destroyFn(temp); temp = nullptr; } catch (...) { /* swallow */ }
            }
        }
    }
    catch (const std::exception& ex) {
        qDebug() << "Metadata collection failed for" << QString::fromStdString(meta.path) << ":" << ex.what();
    }
    catch (...) {
        qDebug() << "Metadata collection failed for" << QString::fromStdString(meta.path);
    }

    m_pluginMetadata.push_back(meta);
    m_metadataByPath[meta.path] = meta;
}

bool PluginRegistry::load(const std::string& pluginPath, uint32_t requiredApi) {
    if (!isSharedLib(pluginPath) || !looksLikePlugin(pluginPath)) return false;
    if (m_loadedByPath.find(pluginPath) != m_loadedByPath.end()) return true; // already
    try {
#ifdef _WIN32
        boost::dll::shared_library lib(
            pluginPath,
            boost::dll::load_mode::load_with_altered_search_path | boost::dll::load_mode::rtld_lazy
        );
#else
        boost::dll::shared_library lib(pluginPath, boost::dll::load_mode::rtld_lazy);
#endif
        if (!lib.has(MSS_PLUGIN_API)) return false;
        auto api = lib.get<mss_av_t*>(MSS_PLUGIN_API)();
        if (api != requiredApi) return false;

        auto make = lib.get<mss_make_t*>(MSS_PLUGIN_API_MAKE);
        auto destroy = lib.get<mss_destroy_t*>(MSS_PLUGIN_API_DESTROY);
        IPlugin* inst = make();
        if (!inst) return false;

        m_loaded.push_back(LoadedPlugin{ std::move(lib), inst, destroy, pluginPath });
        m_loadedByType[inst->type()].push_back(&m_loaded.back());
        m_loadedByPath[pluginPath] = &m_loaded.back();
    }
    catch (const std::exception& ex) {
        qDebug() << "Failed to load plugin from " << QString::fromStdString(pluginPath)
            << ": " << ex.what();
        return false;
    }
    catch (...) {
        qDebug() << "Failed to load plugin from " << QString::fromStdString(pluginPath);
        return false;
    }

    return true;
}

bool PluginRegistry::unload(const std::string& pluginPath) {
    auto it = m_loadedByPath.find(pluginPath);
    if (it == m_loadedByPath.end()) return false;
    auto plugin = it->second;
    if (!plugin) return false;
    auto pluginType = plugin->instance ? plugin->instance->type() : IElement::Type::Source;
    if (plugin->instance && plugin->destroy) {
        try { plugin->destroy(plugin->instance); }
        catch (...) {}
        plugin->instance = nullptr;
    }
    m_loadedByType[pluginType].erase(std::remove_if(m_loadedByType[pluginType].begin(), m_loadedByType[pluginType].end(),
        [&](LoadedPlugin* p) { return p == plugin; }), m_loadedByType[pluginType].end());
    m_loadedByPath.erase(pluginPath);
    m_loaded.erase(std::remove_if(m_loaded.begin(), m_loaded.end(),
        [&](const LoadedPlugin& p) { return p.path == pluginPath; }), m_loaded.end());
    return true;
}

void PluginRegistry::loadAll() {
    for (auto& p : m_pluginMetadata) {
		// check if plugin is to be loaded (settings)
        if (m_settingsController.pluginsSettings().enabledPluginIds.size() > 0) {
            QString pluginPath = QString::fromStdString(p.path);
            if (!m_settingsController.pluginsSettings().enabledPluginIds.contains(pluginPath)) {
                qDebug() << "Skipping plugin '" << p.name << "' as it is not enabled in settings.";
                continue;
            }
		}

        if (!this->load(p.path, FACTORY_API_VERSION)) {
            qDebug() << "Failed to load plugin '" << QString::fromStdString(p.name) << "'";
        }
    }
}

void PluginRegistry::unloadAll() {
    for (auto& p : m_loaded) {
        if (p.instance && p.destroy) { p.destroy(p.instance); p.instance = nullptr; }
    }
    m_loaded.clear();
    m_loadedByPath.clear();
    m_loadedByType.clear();
}

const std::vector<LoadedPlugin*> PluginRegistry::byType(IElement::Type t) const {
    const std::vector<LoadedPlugin*> kEmpty;
    auto it = m_loadedByType.find(t);
    return it == m_loadedByType.end() ? kEmpty : it->second;
}

const std::vector<LoadedPlugin*> PluginRegistry::byPath(const std::string& path) const {
    const std::vector<LoadedPlugin*> kEmpty;
    auto it = m_loadedByPath.find(path);
    return it == m_loadedByPath.end() ? kEmpty : std::vector<LoadedPlugin*>{ it->second };
}

