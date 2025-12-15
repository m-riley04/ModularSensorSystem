#pragma once

#include <QObject>
#include <QDir>
#include <QCoreApplication>
#include "sdk/plugins/isourceplugin.hpp"
#include "sdk/plugins/iprocessorplugin.hpp"

#include "sdk/plugins/pluginloader.hpp"
#include "sdk/plugins/imountplugin.hpp"
#include <string>
#include "settingscontroller.hpp"
#include "core_export.hpp"

class MSS_CORE_API PluginController : public QObject
{
    Q_OBJECT

public:
    explicit PluginController(SettingsController& settingsController, QObject* parent = nullptr);

    void loadPlugins();
    void unloadPlugins();

	void loadPlugin(const QString& pluginId);
	void unloadPlugin(const QString& pluginId);

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
    QDir m_pluginRoot;
    std::vector<std::filesystem::path> m_pluginPaths;
    PluginRegistry m_pluginRegistry;

    void populateSourcePlugins();
    void populateProcessorPlugins();
    void populateMountPlugins();

    std::vector<std::filesystem::path> buildPluginPaths();
};