#pragma once

#include <QObject>
#include <vector>
#include "sdk/plugins/iplugin.hpp"
#include "features/mounts/mount.hpp"

class IMountPlugin : public IPlugin
{
public:
    virtual std::vector<ElementInfo> discover() const = 0;
    virtual Mount* createMount(const std::string& id, QObject* parent) = 0;
    PluginType type() const override { return PluginType::MOUNT; }
};
