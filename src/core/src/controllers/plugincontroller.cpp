#include "controllers/plugincontroller.hpp"

PluginController::PluginController(const std::string& root, QObject* parent)
	: BackendControllerBase("PluginController", parent), m_pluginRoot(root)
{
	m_pluginPaths = buildPluginPaths();

	// Scan and load plugins on initialization
	// TODO: when app settings are implemented, only load plugins that have been enabled
	this->rescanPlugins();
	loadPlugins();
}

void PluginController::rescanPlugins() {
	m_pluginRegistry.scan(m_pluginPaths, FACTORY_API_VERSION);
}

void PluginController::unloadPlugins() {
	// Unload all plugins from registry
	m_pluginRegistry.unloadAll();

	// Clear plugin lists
	m_plugins.clear();
	m_sourcePlugins.clear();
	m_processorPlugins.clear();
	m_mountPlugins.clear();
}

void PluginController::loadPlugin(const QString& pluginPath)
{
	m_pluginRegistry.load(pluginPath.toStdString(), FACTORY_API_VERSION);
}

void PluginController::unloadPlugin(const QString& pluginPath)
{
	m_pluginRegistry.unload(pluginPath.toStdString());
}

void PluginController::loadPlugins()
{
	m_pluginRegistry.loadAll();

	// Populate plugin lists
	populateSourcePlugins();
	populateProcessorPlugins();
	populateMountPlugins();
}

void PluginController::populateSourcePlugins() {
	std::vector<ISourcePlugin*> p = m_pluginRegistry.as<ISourcePlugin>();
	m_sourcePlugins = QList<ISourcePlugin*>(p.begin(), p.end());

	// Append to main plugin list
	for (ISourcePlugin* plugin : m_sourcePlugins) {
		m_plugins.append(plugin);
	}
}

void PluginController::populateProcessorPlugins() {
	std::vector<IProcessorPlugin*> p = m_pluginRegistry.as<IProcessorPlugin>();
	m_processorPlugins = QList<IProcessorPlugin*>(p.begin(), p.end());

	// Append to main plugin list
	for (IProcessorPlugin* plugin : m_processorPlugins) {
		m_plugins.append(plugin);
	}
}

void PluginController::populateMountPlugins() {
	std::vector<IMountPlugin*> p = m_pluginRegistry.as<IMountPlugin>();
	m_mountPlugins = QList<IMountPlugin*>(p.begin(), p.end());

	// Append to main plugin list
	for (IMountPlugin* plugin : m_mountPlugins) {
		m_plugins.append(plugin);
	}
}

ISourcePlugin* PluginController::getSourcePlugin(const QString& pluginId) const
{
	for (ISourcePlugin* plugin : m_sourcePlugins) {
		if (!plugin) continue;
		QString name = QString::fromStdString(plugin->name());
		if (name == pluginId) {
			return plugin;
		}
	}
	return nullptr;
}

IProcessorPlugin* PluginController::getProcessorPlugin(const QString& pluginId) const
{
	for (IProcessorPlugin* plugin : m_processorPlugins) {
		if (!plugin) continue;
		QString name = QString::fromStdString(plugin->name());
		if (name == pluginId) {
			return plugin;
		}
	}
	return nullptr;
}

IMountPlugin* PluginController::getMountPlugin(const QString& pluginId) const
{
	for (IMountPlugin* plugin : m_mountPlugins) {
		if (!plugin) continue;
		QString name = QString::fromStdString(plugin->name());
		if (name == pluginId) {
			return plugin;
		}
	}
	return nullptr;
}

std::vector<std::filesystem::path> PluginController::buildPluginPaths()
{
	std::vector<std::filesystem::path> pluginPaths;

	// Build plugin paths based on types
	std::filesystem::path mountsPluginsPath = m_pluginRoot + "/mounts";
	std::filesystem::path sourcesPluginsPath = m_pluginRoot + "/sources";
	std::filesystem::path processorsPluginsPath = m_pluginRoot + "/processors";
	pluginPaths.push_back(mountsPluginsPath);
	pluginPaths.push_back(sourcesPluginsPath);
	pluginPaths.push_back(processorsPluginsPath);
	return pluginPaths;
}
