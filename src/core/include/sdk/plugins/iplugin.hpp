#pragma once

#include <string>

enum class PluginType {
    Source,
    Processor,
    Mount
};

class IPlugin {
public:
    virtual ~IPlugin() = default;
    virtual std::string name() const = 0;
    virtual uint32_t version() const = 0;
	virtual PluginType type() const = 0;
};
