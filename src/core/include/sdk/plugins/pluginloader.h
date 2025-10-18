#pragma once

#include <boost/dll/shared_library.hpp>
#include <filesystem>
#include <vector>
#include <deque>
#include <unordered_map>
#include <string>
#include <QDebug>
#include "sdk/plugins/factory.h"
#include "sdk/plugins/iplugin.h"

constexpr uint32_t FACTORY_API_VERSION = 0x00010000;

struct LoadedPlugin {
    boost::dll::shared_library lib;
    IPlugin* instance{ nullptr };
    mss_destroy_t* destroy{ nullptr };
    std::string path;
};

class PluginRegistry {
public:
    void scan(const std::vector<std::filesystem::path>& dirs, uint32_t requiredApi) {
        for (auto& d : dirs) if (std::filesystem::exists(d)) scanDir(d, requiredApi);
    }

    const std::deque<LoadedPlugin>& all() const { return m_all; }
    const std::vector<LoadedPlugin*>& sources() const { return byType(PluginType::Source); }
    const std::vector<LoadedPlugin*>& processors() const { return byType(PluginType::Processor); }
    const std::vector<LoadedPlugin*>& mounts()   const { return byType(PluginType::Mount); }

    void unloadAll() {
        // 1) Destroy instances while code is still loaded
        for (auto& p : m_all) {
            if (p.instance && p.destroy) { p.destroy(p.instance); p.instance = nullptr; }
        }
        // 2) Now unload libs by clearing owners
        m_all.clear();
        // 3) Clear non-owning buckets
        m_byType.clear();
    }

    template <typename T> std::vector<T*> as() const {
        std::vector<T*> out;
        out.reserve(m_all.size());
        for (auto& p : m_all) if (auto cast = dynamic_cast<T*>(p.instance)) out.push_back(cast);
        return out;
    }

    ~PluginRegistry() { unloadAll(); }

private:
    const std::vector<LoadedPlugin*>& byType(PluginType t) const {
        static const std::vector<LoadedPlugin*> kEmpty;
        auto it = m_byType.find(t);
        return it == m_byType.end() ? kEmpty : it->second;
    }

    static bool isSharedLib(const std::filesystem::path& p) {
#ifdef _WIN32
        return p.extension() == ".dll";
#elif __APPLE__
        return p.extension() == ".dylib";
#else
        return p.extension() == ".so";
#endif
    }

    static bool looksLikePlugin(const std::filesystem::path& p) {
        // Heuristic: only try files that look like actual plugins to avoid probing dependencies
        const std::string name = p.filename().string();
#ifdef _WIN32
        return name.size() > 10 && name.rfind("Plugin.dll") == name.size() - std::string("Plugin.dll").size();
#elif __APPLE__
        return name.find("Plugin") != std::string::npos && p.extension() == ".dylib";
#else
        return name.find("Plugin") != std::string::npos && p.extension() == ".so";
#endif
    }

    void scanDir(const std::filesystem::path& dir, uint32_t requiredApi) {
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
                if (!lib.has("mss_api")) continue;
                auto api = lib.get<mss_av_t*>("mss_api")();
                if (api != requiredApi) continue;

                auto make = lib.get<mss_make_t*>("mss_make");
                auto destroy = lib.get<mss_destroy_t*>("mss_destroy");
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

    std::deque<LoadedPlugin> m_all;
    std::unordered_map<PluginType, std::vector<LoadedPlugin*>> m_byType;
};