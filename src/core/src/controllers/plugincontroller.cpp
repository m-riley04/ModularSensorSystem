#include "controllers/plugincontroller.hpp"

PluginController::PluginController(SettingsController& settingsController, QObject* parent)
	: BackendControllerBase("PluginController", parent)
	, m_pluginRegistry(settingsController)
	, m_pluginRoot(settingsController.pluginsSettings().pluginsDirectory)
{
	// Ensure m_pluginRoot is a valid absolute directory. Fallback to application plugins dir.
	QString rootAbs = m_pluginRoot.absolutePath();
	if (rootAbs.isEmpty() || !m_pluginRoot.exists()) {
		QDir fallback(QCoreApplication::applicationDirPath() + "/plugins");
		if (!fallback.exists()) {
			fallback.mkpath(fallback.absolutePath());
		}
		m_pluginRoot = fallback;
		rootAbs = m_pluginRoot.absolutePath();
	}

	m_pluginPaths = buildPluginPaths();
	// Scan and load plugins on initialization
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
	auto& pluginMeta = m_pluginRegistry.metadataByPath().at(pluginPath.toStdString());
	if (pluginMeta.type == IElement::Type::Source) {
		ISourcePlugin* plugin = m_pluginRegistry.as < ISourcePlugin
		>().back(); // Get the last loaded source plugin
		m_sourcePlugins.append(plugin);
		m_plugins.append(plugin);
	}
	else if (pluginMeta.type == IElement::Type::Processor) {
		IProcessorPlugin* plugin = m_pluginRegistry.as<IProcessorPlugin>().back(); // Get the last loaded processor plugin
		m_processorPlugins.append(plugin);
		m_plugins.append(plugin);
	}
	else if (pluginMeta.type == IElement::Type::Mount) {
		IMountPlugin* plugin = m_pluginRegistry.as<IMountPlugin>().back(); // Get the last loaded mount plugin
		m_mountPlugins.append(plugin);
		m_plugins.append(plugin);
	}
}

void PluginController::unloadPlugin(const QString& pluginPath)
{
	// Identify the loaded plugin instance by path BEFORE unloading so we can remove
	// the exact pointer from our local lists safely.
	const std::string path = pluginPath.toStdString();
	IPlugin* instancePtr = nullptr;
	if (auto lp = m_pluginRegistry.loadedByPath(path)) {
		instancePtr = lp->instance;
	}

	// Determine plugin type from metadata (if available)
	IElement::Type pluginType = IElement::Type::Source; // default doesn't matter here
	{
		auto it = m_pluginRegistry.metadataByPath().find(path);
		if (it != m_pluginRegistry.metadataByPath().end()) {
			pluginType = it->second.type;
		}
	}

	// Remove from specific type list and aggregate list using pointer equality
	if (instancePtr) {
		if (pluginType == IElement::Type::Source) {
			m_sourcePlugins.erase(std::remove(m_sourcePlugins.begin(), m_sourcePlugins.end(), static_cast<ISourcePlugin*>(instancePtr)), m_sourcePlugins.end());
		}
		else if (pluginType == IElement::Type::Processor) {
			m_processorPlugins.erase(std::remove(m_processorPlugins.begin(), m_processorPlugins.end(), static_cast<IProcessorPlugin*>(instancePtr)), m_processorPlugins.end());
		}
		else if (pluginType == IElement::Type::Mount) {
			m_mountPlugins.erase(std::remove(m_mountPlugins.begin(), m_mountPlugins.end(), static_cast<IMountPlugin*>(instancePtr)), m_mountPlugins.end());
		}

		m_plugins.erase(std::remove(m_plugins.begin(), m_plugins.end(), instancePtr), m_plugins.end());
	}

	// Finally unload from registry
	m_pluginRegistry.unload(path);
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

	// Determine a safe root path
	QString rootAbs = m_pluginRoot.absolutePath();
	if (rootAbs.isEmpty()) {
		rootAbs = QCoreApplication::applicationDirPath() + "/plugins";
	}

	// Build plugin paths based on types
	std::filesystem::path mountsPluginsPath = rootAbs.toStdString() + "/mounts";
	std::filesystem::path sourcesPluginsPath = rootAbs.toStdString() + "/sources";
	std::filesystem::path processorsPluginsPath = rootAbs.toStdString() + "/processors";
	pluginPaths.push_back(sourcesPluginsPath);
	pluginPaths.push_back(processorsPluginsPath);
	// Optionally include mounts if they exist (won't hurt to add regardless)
	pluginPaths.push_back(mountsPluginsPath);
	return pluginPaths;
}
