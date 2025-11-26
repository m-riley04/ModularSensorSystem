#pragma once

#include <string>

enum class PluginType {
    SOURCE,
    PROCESSOR,
    MOUNT
};

class IPlugin {
public:
    virtual ~IPlugin() = default;
    virtual const std::string& name() const = 0;
    virtual const uint32_t version() const = 0;
	virtual const PluginType type() const = 0;
};
