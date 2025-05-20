#pragma once

#include <QObject>
#include <QDir>
#include <plugins/device/ideviceplugin.h>
#include <QPluginLoader>
#include <QCoreApplication>

class PluginController : public QObject
{
    Q_OBJECT
public:
    explicit PluginController(const QString& pluginRoot, QObject* parent = nullptr);

    void loadPlugins();

    QList<IDevicePlugin*> devicePlugins() const { return mDevicePlugins; }

private:
    QList<IDevicePlugin*> mDevicePlugins;
	QString mPluginRoot;

	void loadPlugin(const QString& fullPath, const QString& fileName);
};