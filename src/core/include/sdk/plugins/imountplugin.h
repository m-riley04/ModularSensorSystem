#pragma once

#include <QObject>
#include <vector>
#include "sdk/plugins/iplugin.h"
#include "features/mounts/mount.h"

class IMountPlugin : public IPlugin
{
public:
    virtual std::vector<Mount*> discover() = 0;
    PluginType type() const override { return PluginType::Mount; }
};
