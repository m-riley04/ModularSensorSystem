#pragma once

#include <QObject>
#include <vector>
#include "sdk/plugins/iplugin.h"
#include "features/mounts/mount.h"

struct MountInfo {
    std::string id;
    std::string name;
};

class IMountPlugin : public IPlugin
{
public:
    virtual std::vector<MountInfo> discover() = 0;
    virtual Mount* createMount(const std::string& id, QObject* parent) = 0;
    PluginType type() const override { return PluginType::Mount; }
};
