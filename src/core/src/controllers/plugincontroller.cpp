#include "controllers/plugincontroller.hpp"

PluginController::PluginController(const QString& root, QObject* parent)
	: BackendControllerBase("PluginController", parent), m_pluginRoot(root)
{
	// Scan for plugins on initialization
	// TODO: Make this configurable
	loadPlugins(QList<PluginType>({
		PluginType::SourcePlugin,
		PluginType::ProcessorPlugin,
		PluginType::MountPlugin
	}));
}

void PluginController::loadPlugins(QList<PluginType> pluginTypes)
{
	// Iterate through plugin types
	QString pluginDirName;
	std::vector<std::filesystem::path> pluginPaths = buildPluginPaths(pluginTypes);

	m_pluginRegistry.scan(pluginPaths, FACTORY_API_VERSION);

	// Populate plugin lists
	populateSourcePlugins();
	populateProcessorPlugins();
	populateMountPlugins();
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

void PluginController::populateSourcePlugins() {
	// Unload existing plugins using unload mechanism
	m_sourcePlugins.clear();

	std::vector<ISourcePlugin*> p = m_pluginRegistry.as<ISourcePlugin>();
	m_sourcePlugins = QList<ISourcePlugin*>(p.begin(), p.end());
}

void PluginController::populateProcessorPlugins() {
	// Unload existing plugins using unload mechanism
	m_processorPlugins.clear();

	std::vector<IProcessorPlugin*> p = m_pluginRegistry.as<IProcessorPlugin>();
	m_processorPlugins = QList<IProcessorPlugin*>(p.begin(), p.end());
}

void PluginController::populateMountPlugins()
{
	// Unload existing plugins using unload mechanism
	m_mountPlugins.clear();

	std::vector<IMountPlugin*> p = m_pluginRegistry.as<IMountPlugin>();
	m_mountPlugins = QList<IMountPlugin*>(p.begin(), p.end());
}

QString PluginController::pluginTypeToDirName(PluginType pluginType)
{
	switch (pluginType) {
	case PluginType::SourcePlugin:
		return "sources";
	case PluginType::ProcessorPlugin:
		return "processors";
	case PluginType::MountPlugin:
		return "mounts";
	default:
		return "unknown";
	}
}

std::vector<std::filesystem::path> PluginController::buildPluginPaths(QList<PluginType> pluginTypes)
{
	std::vector<std::filesystem::path> pluginPaths;

	// Build plugin paths based on types
	std::string pluginDirName;
	for (PluginType pluginType : pluginTypes) {
		pluginDirName = pluginTypeToDirName(pluginType).toStdString();
		std::filesystem::path pluginPath = QCoreApplication::applicationDirPath().toStdString() + "/plugins/" + pluginDirName;
		pluginPaths.push_back(pluginPath);
	}

	return pluginPaths;
}
