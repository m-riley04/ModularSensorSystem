#pragma once

#include <QObject>
#include <QDir>
#include <QCoreApplication>
#include "sdk/plugins/isourceplugin.h"
#include "sdk/plugins/iprocessorplugin.h"
#include "controllers/backendcontrollerbase.h"
#include <sdk/plugins/pluginloader.h>

class PluginController : public BackendControllerBase
{
    Q_OBJECT

public:
	enum PluginType {
		SourcePlugin,
		ProcessorPlugin,
        MountPlugin
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
	PluginRegistry mPluginRegistry;

    void populateSourcePlugins();
	void populateProcessorPlugins();

	QString pluginTypeToDirName(PluginType pluginType);
	std::vector<std::filesystem::path> buildPluginPaths(QList<PluginType> pluginTypes);
};