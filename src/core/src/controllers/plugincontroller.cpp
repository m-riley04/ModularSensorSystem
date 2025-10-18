#include "controllers/plugincontroller.h"

PluginController::PluginController(const QString& root, QObject* parent)
	: BackendControllerBase("PluginController", parent), mPluginRoot(root)
{
	// Scan for plugins on initialization
	loadPlugins(QList<PluginType>({
		PluginType::SourcePlugin,
		PluginType::ProcessorPlugin
	}));
}

void PluginController::loadPlugins(QList<PluginType> pluginTypes)
{
	// Iterate through plugin types
	QString pluginDirName;
	for (PluginType pluginType : pluginTypes) {
		switch (pluginType) {
		case PluginType::SourcePlugin:
			pluginDirName = "sources";
			break;
		case PluginType::ProcessorPlugin:
			pluginDirName = "processors";
			break;
		}

		QString pluginPath = QCoreApplication::applicationDirPath() + "/plugins/" + pluginDirName;
		QDir pluginsDir(pluginPath);
		QStringList files = pluginsDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
		for (const QString& fileName : files) {
			if (!QLibrary::isLibrary(fileName))
				continue; // Only consider dynamic libraries

			const QString fullPath = pluginsDir.absoluteFilePath(fileName);

			// Fast pre-check: only attempt to load libraries that actually contain Qt plugin metadata
			QPluginLoader metaProbe(fullPath);
			const QJsonObject meta = metaProbe.metaData();
			if (meta.isEmpty()) {
				// Skip non-Qt plugin libraries (e.g., dependency DLLs like gstreamer, glib, etc.)
				qDebug() << "Skipping non-Qt plugin library in" << pluginDirName << ":" << fileName;
				continue;
			}

			loadPlugin(fullPath, fileName, pluginType);
		}
	}
}

void PluginController::loadPlugin(const QString& fullPath, const QString& fileName, PluginType pluginType)
{
	QPluginLoader loader(fullPath);

	// Guard: if this is not a Qt plugin, don't try to instantiate
	if (loader.metaData().isEmpty()) {
		qDebug() << "Not a Qt plugin (no metadata):" << fileName << "- skipping.";
		return;
	}

	QObject* pluginInstance = loader.instance();
	if (!pluginInstance) {
		qWarning() << "Failed to load plugin" << fileName << ":" << loader.errorString();
		return;
	}

	// Decide which plugin type to load based on the provided pluginType
	switch (pluginType) {
	case PluginType::SourcePlugin:
		loadSourcePlugin(loader, fileName);
		break;
	case PluginType::ProcessorPlugin:
		loadProcessorPlugin(loader, fileName);
		break;
	}
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

void PluginController::loadSourcePlugin(QPluginLoader& loader, QString file)
{
	ISourcePlugin* sourcePlugin = qobject_cast<ISourcePlugin*>(loader.instance());
	if (!sourcePlugin) {
		qWarning() << "Loaded plugin" << file << "does not implement ISourcePlugin interface. Unloading...";
		loader.unload();  // Unload if not the correct plugin type
		return;
	}

	mSourcePlugins.append(sourcePlugin);
}

void PluginController::loadProcessorPlugin(QPluginLoader& loader, QString file)
{
	IProcessorPlugin* processorPlugin = qobject_cast<IProcessorPlugin*>(loader.instance());
	if (!processorPlugin) {
		qWarning() << "Loaded plugin" << file << "does not implement IProcessorPlugin interface. Unloading...";
		loader.unload();  // Unload if not the correct plugin type
		return;
	}

	mProcessorPlugins.append(processorPlugin);
}
