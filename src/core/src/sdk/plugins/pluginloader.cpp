#include "sdk/plugins/pluginloader.hpp"

void PluginRegistry::scan(const std::vector<std::filesystem::path>& dirs, uint32_t requiredApi) {
    for (auto& d : dirs) if (std::filesystem::exists(d)) scanDir(d, requiredApi);
}

void PluginRegistry::unloadAll() {
    for (auto& p : m_all) {
        if (p.instance && p.destroy) { p.destroy(p.instance); p.instance = nullptr; }
    }
    m_all.clear();
    m_byType.clear();
}

const std::vector<LoadedPlugin*>& PluginRegistry::byType(PluginType t) const {
    static const std::vector<LoadedPlugin*> kEmpty;
    auto it = m_byType.find(t);
    return it == m_byType.end() ? kEmpty : it->second;
}

void PluginRegistry::scanDir(const std::filesystem::path& dir, uint32_t requiredApi) {
    for (auto& e : std::filesystem::directory_iterator(dir)) {
        if (!e.is_regular_file() || !isSharedLib(e.path()) || !looksLikePlugin(e.path())) continue;
        try {
            // On Windows ensure the DLL's directory is searched for its dependencies
#ifdef _WIN32
            boost::dll::shared_library lib(
                e.path().string(),
                boost::dll::load_mode::load_with_altered_search_path
            );
#else
            boost::dll::shared_library lib(e.path().string());
#endif
            if (!lib.has(MSS_PLUGIN_API)) continue;
            auto api = lib.get<mss_av_t*>(MSS_PLUGIN_API)();
            if (api != requiredApi) continue;

            auto make = lib.get<mss_make_t*>(MSS_PLUGIN_API_MAKE);
            auto destroy = lib.get<mss_destroy_t*>(MSS_PLUGIN_API_DESTROY);
            IPlugin* inst = make();

            // Bucket by kind
            m_all.push_back(LoadedPlugin{ std::move(lib), inst, destroy, e.path().string() });
            m_byType[inst->type()].push_back(&m_all.back()); // copies, so push then keep ref
        }
        catch (const std::exception& ex) {
            qDebug() << "Failed to load plugin from " << QString::fromStdString(e.path().string())
                << ": " << ex.what();
        }
        catch (...) {
            qDebug() << "Failed to load plugin from " << QString::fromStdString(e.path().string());
        }
    }
}