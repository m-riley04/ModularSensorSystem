#include "plugincontroller.h"

PluginController::PluginController(const QString& root, QObject* parent)
	: QObject(parent), mPluginRoot(root)
{
	// Scan for plugins on initialization
	loadPlugins();
}

void PluginController::loadPlugins()
{
	QString pluginPath = QCoreApplication::applicationDirPath() + "/plugins/devices";
	QDir pluginsDir(pluginPath);
	QStringList files = pluginsDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
	for (const QString& fileName : files) {
		if (QLibrary::isLibrary(fileName)) {  // check extension .dll on Windows
			QString fullPath = pluginsDir.absoluteFilePath(fileName);
			loadDevicePlugin(fullPath, fileName);
		}
	}
}

IDevicePlugin* PluginController::getDevicePlugin(const QString& pluginId) const
{
	for (IDevicePlugin* plugin : mDevicePlugins) {
		QString pluginName = plugin->pluginName();
		if (plugin->pluginName() == pluginId) {
			return plugin;
		}
	}
	return nullptr;  // Return nullptr if not found
}

IProcessorPlugin* PluginController::getProcessorPlugin(const QString& pluginId) const
{
	for (IProcessorPlugin* plugin : mProcessorPlugins) {
		QString pluginName = plugin->name();
		if (plugin->name() == pluginId) {
			return plugin;
		}
	}
	return nullptr;  // Return nullptr if not found
}

void PluginController::loadDevicePlugin(const QString& fullPath, const QString& fileName)
{
	QPluginLoader loader(fullPath);
	QObject* pluginInstance = loader.instance();
	if (!pluginInstance) {
		qWarning() << "Failed to load plugin" << fileName << ":" << loader.errorString();
		return;
	}
	IDevicePlugin* devicePlugin = qobject_cast<IDevicePlugin*>(pluginInstance);
	if (!devicePlugin) {
		qWarning() << "Loaded plugin" << fileName << "does not implement IDevicePlugin interface. Unloading...";
		loader.unload();  // Unload if not the correct plugin type
		return;
	}

	mDevicePlugins.append(devicePlugin);
}

void PluginController::loadProcessorPlugin(const QString& fullPath, const QString& fileName)
{
	QPluginLoader loader(fullPath);
	QObject* pluginInstance = loader.instance();
	if (!pluginInstance) {
		qWarning() << "Failed to load plugin" << fileName << ":" << loader.errorString();
		return;
	}
	IProcessorPlugin* processorPlugin = qobject_cast<IProcessorPlugin*>(pluginInstance);
	if (!processorPlugin) {
		qWarning() << "Loaded plugin" << fileName << "does not implement IProcessorPlugin interface. Unloading...";
		loader.unload();  // Unload if not the correct plugin type
		return;
	}

	mProcessorPlugins.append(processorPlugin);
}
