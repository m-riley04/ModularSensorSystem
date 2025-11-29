#pragma once

#include <boost/dll/shared_library.hpp>
#include <filesystem>
#include <vector>
#include <deque>
#include <unordered_map>
#include <string>
#include <QDebug>
#include "sdk/plugins/factory.hpp"
#include "sdk/plugins/iplugin.hpp"
#include "utils/plugins_utils.hpp"
#include "features/ielement.hpp"

/**
 * @brief Represents a loaded plugin.
 */
struct LoadedPlugin {
    boost::dll::shared_library lib;
    IPlugin* instance{ nullptr };
    mss_destroy_t* destroy{ nullptr };
    std::string path;
};

class PluginRegistry {
public:
    ~PluginRegistry() { unloadAll(); }

    /**
	 * @brief Scans the specified directories for plugins.
     * @param dirs The directories to scan for plugins.
     * @param requiredApi The required API version that the plugins must match.
     */
    void scan(const std::vector<std::filesystem::path>& dirs, uint32_t requiredApi);

	const std::deque<std::string>& pluginPaths() const { return m_pluginPaths; }
    const std::deque<LoadedPlugin>& all() const { return m_loaded; }
    const std::vector<LoadedPlugin*>& sources() const { return byType(IElement::Type::Source); }
    const std::vector<LoadedPlugin*>& processors() const { return byType(IElement::Type::Processor); }
    const std::vector<LoadedPlugin*>& mounts() const { return byType(IElement::Type::Mount); }

    bool load(const std::string& pluginPath, uint32_t requiredApi);
    bool unload(const std::string& pluginPath);

    void loadAll();
    void unloadAll();

    /**
	 * @brief Casts all loaded plugins to the specified type T and, if valid, returns them as a vector.
     * @tparam T 
     * @return 
     */
    template <typename T> std::vector<T*> as() const {
        std::vector<T*> out;
        out.reserve(m_loaded.size());
        for (auto& p : m_loaded) if (auto cast = dynamic_cast<T*>(p.instance)) out.push_back(cast);
        return out;
    }

private:
	std::deque<std::string> m_pluginPaths;
    std::deque<LoadedPlugin> m_loaded;
    std::unordered_map<IElement::Type, std::vector<LoadedPlugin*>> m_loadedByType;
	std::unordered_map<std::string, LoadedPlugin*> m_loadedByPath;

    /**
	 * @brief Gets loaded plugins by type.
     * @param t The type of elements to filter by.
     * @return A vector of loaded plugins of the specified type.
     */
    const std::vector<LoadedPlugin*> byType(IElement::Type t) const;

	const std::vector<LoadedPlugin*> byPath(const std::string& path) const;

    /**
     * @brief Scans a directory for plugins and populates the list of plugin paths.
     * @param dir The directory to scan for plugins.
     * @param requiredApi The required API version that the plugins must match.
     */
    void scanDir(const std::filesystem::path& dir, uint32_t requiredApi);

};