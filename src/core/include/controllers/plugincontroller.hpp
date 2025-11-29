#pragma once

#include <QObject>
#include <QDir>
#include <QCoreApplication>
#include "sdk/plugins/isourceplugin.hpp"
#include "sdk/plugins/iprocessorplugin.hpp"
#include "controllers/backendcontrollerbase.hpp"
#include "sdk/plugins/pluginloader.hpp"
#include "sdk/plugins/imountplugin.hpp"
#include <string>

class PluginController : public BackendControllerBase
{
    Q_OBJECT

public:
    explicit PluginController(const std::string& pluginRoot, QObject* parent = nullptr);

    void loadPlugins();
    void unloadPlugins();

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
    std::string m_pluginRoot;
    PluginRegistry m_pluginRegistry;

    void loadSourcePlugins();
    void loadProcessorPlugins();
    void loadMountPlugins();

    std::vector<std::filesystem::path> buildPluginPaths();
};