#include "controllers/plugincontroller.h"

PluginController::PluginController(const QString& root, QObject* parent)
	: BackendControllerBase("PluginController", parent), mPluginRoot(root)
{
	// Scan for plugins on initialization
	// TODO: Make this configurable
	loadPlugins(QList<PluginType>({
		PluginType::SourcePlugin,
		PluginType::ProcessorPlugin
	}));
}

void PluginController::loadPlugins(QList<PluginType> pluginTypes)
{
	// Iterate through plugin types
	QString pluginDirName;
	std::vector<std::filesystem::path> pluginPaths = buildPluginPaths(pluginTypes);

	mPluginRegistry.scan(pluginPaths, FACTORY_API_VERSION);

	// Populate plugin lists
	populateSourcePlugins();
	populateProcessorPlugins();
}

ISourcePlugin* PluginController::getSourcePlugin(const QString& pluginId) const
{
	for (ISourcePlugin* plugin : mSourcePlugins) {
		QString pluginName = QString::fromStdString(plugin->name());
		if (plugin->name() == pluginId) {
			return plugin;
		}
	}
	return nullptr;  // Return nullptr if not found
}

IProcessorPlugin* PluginController::getProcessorPlugin(const QString& pluginId) const
{
	for (IProcessorPlugin* plugin : mProcessorPlugins) {
		QString pluginName = QString::fromStdString(plugin->name());
		if (plugin->name() == pluginId) {
			return plugin;
		}
	}
	return nullptr;  // Return nullptr if not found
}

void PluginController::populateSourcePlugins() {
	// Unload existing plugins using unload mechanism
	mSourcePlugins.clear();

	std::vector<ISourcePlugin*> p = mPluginRegistry.as<ISourcePlugin>();
	mSourcePlugins = QList<ISourcePlugin*>(p.begin(), p.end());
}

void PluginController::populateProcessorPlugins() {
	// Unload existing plugins using unload mechanism
	mProcessorPlugins.clear();

	std::vector<IProcessorPlugin*> p = mPluginRegistry.as<IProcessorPlugin>();
	mProcessorPlugins = QList<IProcessorPlugin*>(p.begin(), p.end());
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
