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
	 * @brief Scans the specified directories for plugins and loads them if they match the required API version.
     * @param dirs The directories to scan for plugins.
     * @param requiredApi The required API version that the plugins must match.
     */
    void scan(const std::vector<std::filesystem::path>& dirs, uint32_t requiredApi);

    const std::deque<LoadedPlugin>& all() const { return m_all; }
    const std::vector<LoadedPlugin*>& sources() const { return byType(IElement::Type::Source); }
    const std::vector<LoadedPlugin*>& processors() const { return byType(IElement::Type::Processor); }
    const std::vector<LoadedPlugin*>& mounts() const { return byType(IElement::Type::Mount); }

    /**
     * @brief Unloads/frees all loaded plugins.
     */
    void unloadAll();

    /**
	 * @brief Casts all loaded plugins to the specified type T and, if valid, returns them as a vector.
     * @tparam T 
     * @return 
     */
    template <typename T> std::vector<T*> as() const {
        std::vector<T*> out;
        out.reserve(m_all.size());
        // TODO: fix this cast or something to do with mount plugins
        for (auto& p : m_all) if (auto cast = dynamic_cast<T*>(p.instance)) out.push_back(cast);
        return out;
    }

private:
    std::deque<LoadedPlugin> m_all;
    std::unordered_map<IElement::Type, std::vector<LoadedPlugin*>> m_byType;

    /**
	 * @brief Gets loaded plugins by type.
     * @param t 
     * @return 
     */
    const std::vector<LoadedPlugin*>& byType(IElement::Type t) const;

    /**
	 * @brief Scans a directory for plugins and loads them if they match the required API version.
     * @param dir The directory to scan for plugins.
     * @param requiredApi The required API version that the plugins must match.
     */
    void scanDir(const std::filesystem::path& dir, uint32_t requiredApi);

};