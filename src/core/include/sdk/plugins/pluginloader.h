#pragma once

#include <boost/dll/shared_library.hpp>
#include <filesystem>
#include <vector>
#include "sdk/plugins/factory.h"
#include "sdk/plugins/isourceplugin.h"

struct LoadedPlugin {
    boost::dll::shared_library lib;
    ISourcePlugin* instance{ nullptr };
    mss_destroy_t* destroy{ nullptr };
    std::string path;
};

class PluginRegistry {
public:
    void scan(const std::filesystem::path& dir, uint32_t requiredApi) {
        for (auto& p : std::filesystem::directory_iterator(dir)) {
            if (!p.is_regular_file()) continue;
            if (!is_shared_lib(p.path())) continue;

            try {
                boost::dll::shared_library lib(p.path().string());
                auto api = lib.get<mss_av_t*>("mss_api")();
                if (api != requiredApi) continue;

                auto make = lib.get<mss_make_t*>("mss_make");
                auto destroy = lib.get<mss_destroy_t*>("mss_destroy");
                ISourcePlugin* inst = make();

                LoadedPlugin lp{ std::move(lib), inst, destroy, p.path().string() };
                plugins_.push_back(std::move(lp));
            }
            catch (...) {
                // log and skip
            }
        }
    }

    const std::vector<LoadedPlugin>& all() const { return plugins_; }

    ~PluginRegistry() {
        for (auto& p : plugins_) if (p.instance && p.destroy) p.destroy(p.instance);
    }

private:
    static bool is_shared_lib(const std::filesystem::path& f) {
#ifdef _WIN32
        return f.extension() == ".dll";
#elif __APPLE__
        return f.extension() == ".dylib";
#else
        return f.extension() == ".so";
#endif
    }
    std::vector<LoadedPlugin> plugins_;
};