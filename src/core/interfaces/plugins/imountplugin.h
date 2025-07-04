#pragma once
#include <QObject>
#include <features/mounts/Mount/mount.h>

class IMountPlugin
{
public:
    virtual ~IMountPlugin() = default;
    virtual QString id() const = 0;
    virtual QString pluginName() const = 0;
    virtual QList<Mount*> discover(QObject* parent = nullptr) = 0;
};
Q_DECLARE_INTERFACE(IMountPlugin, "com.mss.IMountPlugin/1.0")