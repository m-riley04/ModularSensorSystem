#pragma once

#include <string>

class IPlugin {
public:
    virtual ~IPlugin() = default;
    virtual std::string name() const = 0;
    virtual uint32_t version() const = 0;
};
