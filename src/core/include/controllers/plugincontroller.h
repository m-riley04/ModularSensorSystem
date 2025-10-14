#pragma once

#include <QObject>
#include <QDir>
#include <QPluginLoader>
#include <QCoreApplication>
#include <interfaces/plugins/isourceplugin.h>
#include <interfaces/plugins/iprocessorplugin.h>
#include "controllers/backendcontrollerbase.h"

class PluginController : public BackendControllerBase
{
    Q_OBJECT

public:
	enum PluginType {
		SourcePlugin,
		ProcessorPlugin
	};

public:
    explicit PluginController(const QString& pluginRoot, QObject* parent = nullptr);

    void loadPlugins(QList<PluginType> pluginType);
    void loadPlugin(const QString& fullPath, const QString& fileName, PluginType pluginType);

    ISourcePlugin* getSourcePlugin(const QString& pluginId) const;
    IProcessorPlugin* getProcessorPlugin(const QString& pluginId) const;

    QList<ISourcePlugin*> sourcePlugins() const { return mSourcePlugins; }
    QList<IProcessorPlugin*> processorPlugins() const { return mProcessorPlugins; }

private:
    QList<ISourcePlugin*> mSourcePlugins;
    QList<IProcessorPlugin*> mProcessorPlugins;
	QString mPluginRoot;

    void loadSourcePlugin(QPluginLoader& loader, QString file);
    void loadProcessorPlugin(QPluginLoader& loader, QString file);
};