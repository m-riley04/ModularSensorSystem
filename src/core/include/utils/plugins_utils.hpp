#pragma once

#include <filesystem>
#include <string>

constexpr const char* MSS_PLUGIN_API = "mss_api"; // Function to get the API version;
constexpr const char* MSS_PLUGIN_API_MAKE = "mss_make"; // Function to create a plugin instance;
constexpr const char* MSS_PLUGIN_API_DESTROY = "mss_destroy"; // Function to destroy a plugin instance
constexpr const char* MSS_PLUGIN_SUFFIX = "Plugin"; // the suffix that plugin filenames should contain (before the extension)
constexpr unsigned int MSS_PLUGIN_NAME_MIN_LENGTH = 10; // Minimum length of plugin filename on Windows (to fit "Plugin.dll")
constexpr uint32_t FACTORY_API_VERSION = 0x00010000; // Version of the factory API

/**
 * @brief Checks if the given path has the extension of a shared library for the current platform.
 * @param p The filesystem path to check.
 * @return true if the path corresponds to a shared library, false otherwise
 */
inline static bool isSharedLib(const std::filesystem::path& p) {
#ifdef _WIN32
    return p.extension() == ".dll";
#elif __APPLE__
    return p.extension() == ".dylib";
#else
    return p.extension() == ".so";
#endif
}

/**
 * @brief Heuristically determines whether the given filesystem path looks like a plugin file.
 * @param p The filesystem path to test. The function inspects the filename and extension to decide whether it resembles a plugin.
 * @return true if the path appears to be a plugin file according to platform-specific heuristics; otherwise false. On Windows, returns true when the filename ends with "Plugin.dll" (and the name is longer than 10 characters). On macOS, returns true when the filename contains "Plugin" and the extension is ".dylib". On other Unix-like systems, returns true when the filename contains "Plugin" and the extension is ".so".
 */
static bool looksLikePlugin(const std::filesystem::path& p) {
    const std::string name = p.filename().string();
#ifdef _WIN32
    std::string fullSuffix = MSS_PLUGIN_SUFFIX + std::string(".dll");
    return name.size() > MSS_PLUGIN_NAME_MIN_LENGTH && name.rfind(fullSuffix) == name.size() - fullSuffix.size();
#elif __APPLE__
    return name.find(MSS_PLUGIN_SUFFIX) != std::string::npos && p.extension() == ".dylib";
#else
    return name.find(MSS_PLUGIN_SUFFIX) != std::string::npos && p.extension() == ".so";
#endif
}