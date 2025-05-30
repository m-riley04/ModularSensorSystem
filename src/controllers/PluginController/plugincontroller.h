#pragma once

#include <QObject>
#include <QDir>
#include <QPluginLoader>
#include <QCoreApplication>
#include <interfaces/plugins/ideviceplugin.h>
#include <interfaces/plugins/iprocessorplugin.h>

class PluginController : public QObject
{
    Q_OBJECT

public:
	enum PluginType {
		DevicePlugin,
		ProcessorPlugin
	};

public:
    explicit PluginController(const QString& pluginRoot, QObject* parent = nullptr);

    void loadPlugins(QList<PluginType> pluginType);
    void loadPlugin(const QString& fullPath, const QString& fileName, PluginType pluginType);

    IDevicePlugin* getDevicePlugin(const QString& pluginId) const;
    IProcessorPlugin* getProcessorPlugin(const QString& pluginId) const;

    QList<IDevicePlugin*> devicePlugins() const { return mDevicePlugins; }
    QList<IProcessorPlugin*> processorPlugins() const { return mProcessorPlugins; }

private:
    QList<IDevicePlugin*> mDevicePlugins;
    QList<IProcessorPlugin*> mProcessorPlugins;
	QString mPluginRoot;

    void loadDevicePlugin(QPluginLoader& loader, QString file);
    void loadProcessorPlugin(QPluginLoader& loader, QString file);
};