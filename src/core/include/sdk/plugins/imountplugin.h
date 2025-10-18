#pragma once

#include <QObject>
#include <vector>
#include "sdk/plugins/ielementplugin.h"
#include "features/mounts/mount.h"

class IMountPlugin : public IElementPlugin
{
public:
    virtual std::vector<Mount*> discover() = 0;
};

Q_DECLARE_INTERFACE(IMountPlugin, "com.mss.IMountPlugin/1.0")