#pragma once

#include <QObject>
#include <vector>
#include "sdk/plugins/iplugin.hpp"
#include "features/mounts/mount.hpp"

class IMountPlugin : public IPlugin
{
public:
    virtual const std::vector<ElementInfo> discover() const = 0;
    virtual Mount* createMount(const std::string& id, QObject* parent) = 0;
    virtual const PluginType type() const override final { return PluginType::MOUNT; }
};
