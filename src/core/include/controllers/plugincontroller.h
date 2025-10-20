#pragma once

#include <QObject>
#include <QDir>
#include <QCoreApplication>
#include "sdk/plugins/isourceplugin.h"
#include "sdk/plugins/iprocessorplugin.h"
#include "controllers/backendcontrollerbase.h"
#include <sdk/plugins/pluginloader.h>
#include <sdk/plugins/imountplugin.h>

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
	IMountPlugin* getMountPlugin(const QString& pluginId) const;

    QList<ISourcePlugin*> sourcePlugins() const { return m_sourcePlugins; }
    QList<IProcessorPlugin*> processorPlugins() const { return m_processorPlugins; }
    QList<IMountPlugin*> mountPlugins() const { return m_mountPlugins; }

private:
    QList<ISourcePlugin*> m_sourcePlugins;
    QList<IProcessorPlugin*> m_processorPlugins;
	QList<IMountPlugin*> m_mountPlugins;
	QString m_pluginRoot;
	PluginRegistry m_pluginRegistry;

    void populateSourcePlugins();
	void populateProcessorPlugins();
	void populateMountPlugins();

	QString pluginTypeToDirName(PluginType pluginType);
	std::vector<std::filesystem::path> buildPluginPaths(QList<PluginType> pluginTypes);
};