#pragma once

#include <string>
#include "features/ielement.hpp"
#include "core_export.hpp"

#define MSS_DEFAULT_PLUGIN_AUTHOR "Unknown Author"
#define MSS_DEFAULT_PLUGIN_NAME "Unnamed Plugin"
#define MSS_DEFAULT_PLUGIN_DESCRIPTION "No description provided."
#define MSS_DEFAULT_PLUGIN_VERSION 0x00010000

/**
 * @brief Represents a plugin that can be loaded into the system.
 * Contains boilerplate metadata about the plugin.
 */
class MSS_CORE_API IPlugin {
public:
    virtual ~IPlugin() = default;
    virtual const std::string name() const = 0; // Let it return by value.
    virtual const uint32_t version() const = 0;
    virtual const std::string description() const = 0;
    virtual const IElement::Type type() const = 0;
    virtual const std::string author() const = 0;
	virtual const bool isCore() const = 0;
};
