#pragma once

#include <QObject>
#include <QDir>
#include <plugins/device/ideviceplugin.h>
#include <QPluginLoader>
#include <QCoreApplication>
#include <plugins/processors/iprocessorplugin.h>

class PluginController : public QObject
{
    Q_OBJECT
public:
    explicit PluginController(const QString& pluginRoot, QObject* parent = nullptr);

    void loadPlugins();
    IDevicePlugin* getDevicePlugin(const QString& pluginId) const;
    IProcessorPlugin* getProcessorPlugin(const QString& pluginId) const;

    QList<IDevicePlugin*> devicePlugins() const { return mDevicePlugins; }
    QList<IProcessorPlugin*> processorPlugins() const { return mProcessorPlugins; }

private:
    QList<IDevicePlugin*> mDevicePlugins;
    QList<IProcessorPlugin*> mProcessorPlugins;
	QString mPluginRoot;

    void loadDevicePlugin(const QString& fullPath, const QString& fileName);
    void loadProcessorPlugin(const QString& fullPath, const QString& fileName);
};