#pragma once

#include <QObject>
#include <QDir>
#include <QCoreApplication>
#include "sdk/plugins/isourceplugin.hpp"
#include "sdk/plugins/iprocessorplugin.hpp"
#include "controllers/backendcontrollerbase.hpp"
#include "sdk/plugins/pluginloader.hpp"
#include "sdk/plugins/imountplugin.hpp"

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

	const PluginRegistry& registry() const { return m_pluginRegistry; }

    // Return raw pointers; nullptr when not found
    ISourcePlugin* getSourcePlugin(const QString& pluginId) const;
    IProcessorPlugin* getProcessorPlugin(const QString& pluginId) const;
    IMountPlugin* getMountPlugin(const QString& pluginId) const;

	const QList<IPlugin*>& plugins() const { return m_plugins; }
    const QList<ISourcePlugin*>& sourcePlugins() const { return m_sourcePlugins; }
    const QList<IProcessorPlugin*>& processorPlugins() const { return m_processorPlugins; }
    const QList<IMountPlugin*>& mountPlugins() const { return m_mountPlugins; }

public slots:
	void rescanPlugins();

private:
	QList<IPlugin*> m_plugins;
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