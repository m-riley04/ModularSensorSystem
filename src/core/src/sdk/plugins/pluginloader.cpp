#include "sdk/plugins/pluginloader.hpp"

void PluginRegistry::scan(const std::vector<std::filesystem::path>& dirs, uint32_t requiredApi) {
    for (auto& d : dirs) if (std::filesystem::exists(d)) scanDir(d, requiredApi);
}

void PluginRegistry::loadAll()
{
    for (auto& p : m_pluginPaths) {
        this->load(p, FACTORY_API_VERSION);
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

bool PluginRegistry::unload(const std::string& pluginPath) {
	auto plugin = m_loadedByPath.at(pluginPath);
	auto pluginType = plugin->instance->type();
    if (plugin->instance && plugin->destroy) {
        plugin->destroy(plugin->instance);
        plugin->instance = nullptr;
    }
	m_loadedByType[pluginType].erase(std::remove_if(m_loadedByType[pluginType].begin(), m_loadedByType[pluginType].end(),
        [&](LoadedPlugin* p) { return p == plugin; }), m_loadedByType[pluginType].end());
	m_loadedByPath.erase(pluginPath);
    m_loaded.erase(std::remove_if(m_loaded.begin(), m_loaded.end(),
        [&](const LoadedPlugin& p) { return p.path == pluginPath; }), m_loaded.end());
	return true;
}

bool PluginRegistry::load(const std::string& pluginPath, uint32_t requiredApi)
{
    if (!isSharedLib(pluginPath) || !looksLikePlugin(pluginPath)) return false;
    try {
        // On Windows ensure the DLL's directory is searched for its dependencies
#ifdef _WIN32
        boost::dll::shared_library lib(
            pluginPath,
            boost::dll::load_mode::load_with_altered_search_path
        );
#else
        boost::dll::shared_library lib(e.path().string());
#endif
        if (!lib.has(MSS_PLUGIN_API)) return false;
        auto api = lib.get<mss_av_t*>(MSS_PLUGIN_API)();
        if (api != requiredApi) return false;

        auto make = lib.get<mss_make_t*>(MSS_PLUGIN_API_MAKE);
        auto destroy = lib.get<mss_destroy_t*>(MSS_PLUGIN_API_DESTROY);
        IPlugin* inst = make();

        // Bucket by kind
        m_loaded.push_back(LoadedPlugin{ std::move(lib), inst, destroy, pluginPath });
        m_loadedByType[inst->type()].push_back(&m_loaded.back()); // copies, so push then keep ref
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

const std::vector<LoadedPlugin*> PluginRegistry::byType(IElement::Type t) const {
    const std::vector<LoadedPlugin*> kEmpty;
    auto it = m_loadedByType.find(t);
    return it == m_loadedByType.end() ? kEmpty : it->second;
}

const std::vector<LoadedPlugin*> PluginRegistry::byPath(const std::string& path) const
{
    const std::vector<LoadedPlugin*> kEmpty;
    auto it = m_loadedByPath.find(path);
	return it == m_loadedByPath.end() ? kEmpty : std::vector<LoadedPlugin*>{ it->second };
}

void PluginRegistry::scanDir(const std::filesystem::path& dir, uint32_t requiredApi) {
    for (auto& e : std::filesystem::directory_iterator(dir)) {
        if (!e.is_regular_file() || !isSharedLib(e.path()) || !looksLikePlugin(e.path())) continue;
        if (!this->load(e.path().string(), requiredApi)) {
            qDebug() << "Failed to load plugin from " << QString::fromStdString(e.path().string());
        }
    }
}