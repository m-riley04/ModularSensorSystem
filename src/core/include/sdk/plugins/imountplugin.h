#pragma once

#include <QObject>
#include <vector>
#include "sdk/plugins/iplugin.h"
#include "features/mounts/mount.h"

class IMountPlugin : public IPlugin
{
public:
    virtual std::vector<Mount*> discover() = 0;
};

Q_DECLARE_INTERFACE(IMountPlugin, "com.mss.IMountPlugin/1.0")