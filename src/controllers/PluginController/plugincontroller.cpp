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
			loadPlugin(fullPath, fileName);
		}
	}
}

void PluginController::loadPlugin(const QString& fullPath, const QString& fileName)
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
}
