#pragma once

#include <QObject>
#include <QDir>
#include <plugins/device/ideviceplugin.h>
#include <QPluginLoader>

class PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager(const QString& pluginRoot, QObject* parent = nullptr);

    QList<IDevicePlugin*> devicePlugins() const { return mDevicePlugins; }

private:
    QList<IDevicePlugin*> mDevicePlugins;
};