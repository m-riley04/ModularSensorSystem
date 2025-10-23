#include "controllers/presetscontroller.h"

PresetsController::PresetsController(const QString& dir, QObject *parent)
	: BackendControllerBase("PresetsController", parent), mPresetsDir(dir)
{
	// Check if directory exists
	QDir presetsDir(mPresetsDir);
	if (!presetsDir.exists()) {
		qWarning() << "Presets directory does not exist:" << mPresetsDir;
		// Create it
		if (!presetsDir.mkpath(mPresetsDir)) { // TODO: More error checking if this happens
			qWarning() << "Failed to create presets directory:" << mPresetsDir;
			return;
		}
	}

	// Scan directory for presets
	scanForPresets(mPresetsDir);
}

PresetsController::~PresetsController()
{}

void PresetsController::savePreset(QString name, QList<Source*> activeSources, QString dirPath)
{
	// Check dirPath
	if (dirPath.isEmpty()) {
		dirPath = mPresetsDir;
	}

	QList<SourcePreset> sourcePresets;
	// Iterate each source
	for (auto source : activeSources)
	{
		QJsonObject config;
		if (auto icfg = qobject_cast<IConfigurableSource*>(source)) {
			config = icfg->saveSettings();
		}

		SourcePreset preset {
			QByteArray::fromStdString(source->id()),
			QString::fromStdString(source->name()),
			source->pluginId(),
			source->type(),
			config
		};

		sourcePresets.append(preset);
	}

	QString fullPath = dirPath + "/" + name + ".json";
	Preset preset{
		name,
		fullPath,
		sourcePresets
	};

	// Save preset to file
	QFile file(fullPath);
	if (!file.open(QIODevice::WriteOnly)) {
		qWarning() << "Failed to open file for writing:" << file.errorString();
		return;
	}
	QJsonDocument doc;
	QJsonObject obj;
	obj.insert("name", preset.name);
	obj.insert("sources", sourcePresetsToJson(sourcePresets));
	doc.setObject(obj);

	file.write(doc.toJson());

	// Append the preset if the writing finished successfully
	mPresets.append(preset);

	emit presetSaved(fullPath);
}

void PresetsController::loadPreset(const QString& path, SourceController* sourceController, PluginController* pluginController)
{
	// Check if the file exists
	if (!QFile::exists(path)) {
		qWarning() << "Preset file does not exist:" << path;
		return;
	}

	// Load preset from file
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly)) {
		qWarning() << "Failed to open file for reading:" << file.errorString();
		return;
	}

	// Parse JSON
	QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
	if (doc.isNull()) {
		qWarning() << "Failed to parse JSON:" << path;
		return;
	}
	QJsonObject obj = doc.object();
	if (!obj.contains("name") || !obj.contains("sources")) {
		qWarning() << "Invalid preset format:" << path;
		return;
	}

	// Remove previously loaded sources/settings
	sourceController->clearSources();

	QString name = obj["name"].toString();
	QJsonArray sourcesArray = obj["sources"].toArray();
	QList<SourcePreset> sourcePresets = jsonToSourcePresets(sourcesArray);

	// Iterate each source preset and load it into the source controller
	for (const SourcePreset& sourcePreset : sourcePresets)
	{
		// Clear the current source list
		sourceController->clearSources();

		// Get the plugin for the source
		ISourcePlugin* plugin = pluginController->getSourcePlugin(sourcePreset.pluginId);
		if (!plugin) {
			qWarning() << "Plugin is null: aborting source load";
			return;
		}

		// Create SourceInfo object for the source
		SourceInfo sourceInfo;
		sourceInfo.id = sourcePreset.sourceId;
		sourceInfo.displayName = sourcePreset.sourceName.toStdString();
		sourceInfo.type = sourcePreset.sourceType;

		// Create the source
		Source* source = sourceController->addSource(plugin, sourceInfo);
		if (!source) {
			qWarning() << "Source is null: aborting source load";
			return;
		}
		
		// Load the preset into the source
		if (auto icfg = qobject_cast<IConfigurableSource*>(source)) {
			icfg->loadSettings(sourcePreset.settings);
		}
		else {
			qWarning() << "Source is not configurable:" << source->id();
			continue;
		}
	}

	emit presetLoaded(path);
}

void PresetsController::removePreset(QString filePath)
{
	// Get preset file
	QFile presetFile(filePath);
	if (!presetFile.exists()) {
		qWarning() << "Cannot remove preset at " + filePath + ": file does not exist";
		return;
	}

	// Remove preset from in-memory list
	mPresets.removeIf([this, filePath](Preset preset) {
		return preset.path == filePath;
		});

	// Remove file
	if (!presetFile.remove()) {
		qWarning() << "Unable to remove preset: error in removing";
		return;
	}

	emit presetRemoved(filePath);
}

void PresetsController::scanForPresets(QString presetDir)
{
	// Check passed dir
	if (presetDir.isEmpty()) {
		presetDir = mPresetsDir;
	}

	// Clear list of presets
	mPresets.clear();

	// Scan the presets directory
	QDir dir(presetDir);
	for (QString& file : dir.entryList(QDir::Files))
	{
		QString filePath = dir.absoluteFilePath(file);

		// Load preset into memory
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly)) {
			qWarning() << "Failed to open file for reading:" << file.errorString();
			continue;
		}
		QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
		if (doc.isNull()) {
			qWarning() << "Failed to parse JSON:" << filePath;
			continue;
		}
		QJsonObject obj = doc.object();
		if (!obj.contains("name") || !obj.contains("sources")) {
			qWarning() << "Invalid preset format:" << filePath;
			continue;
		}
		QString name = obj["name"].toString();
		QJsonArray sourcesArray = obj["sources"].toArray();
		QList<SourcePreset> sourcePresets;
		for (const QJsonValue& value : sourcesArray)
		{
			QJsonObject sourceObj = value.toObject();
			if (!sourceObj.contains("id") || !sourceObj.contains("name") || !sourceObj.contains("plugin_id") || !sourceObj.contains("type") || !sourceObj.contains("config")) {
				qWarning() << "Invalid source preset format:" << filePath;
				continue;

				QByteArray sourceId = QByteArray(sourceObj["id"].toString().toStdString());
				QString sourceName = sourceObj["source_name"].toString();
				QString pluginId = sourceObj["plugin_id"].toString();
				Source::Type sourceType = static_cast<Source::Type>(sourceObj["type"].toInt());
				QJsonObject config = sourceObj["config"].toObject();
				SourcePreset preset{
					sourceId,
					sourceName,
					pluginId,
					sourceType,
					config
				};
				sourcePresets.append(preset);
			}
		}

		Preset preset{
			name,
			filePath,
			sourcePresets
		};

		// Finally, add the preset to the list
		mPresets.append(preset);
	}

	emit presetsScanned(mPresets);
}

QJsonArray PresetsController::sourcePresetsToJson(const QList<SourcePreset>& sourcePresets)
{
	QJsonArray array = QJsonArray();
	for (auto& sourcePreset : sourcePresets)
	{
		QJsonObject obj;
		obj["id"] = QString::fromStdString(sourcePreset.sourceId.toStdString());
		obj["name"] = sourcePreset.sourceName;
		obj["plugin_id"] = sourcePreset.pluginId;
		obj["type"] = sourcePreset.sourceType;
		obj["config"] = sourcePreset.settings;
		array.append(obj);
	}
	return array;
}

QList<SourcePreset> PresetsController::jsonToSourcePresets(const QJsonArray& jsonArray)
{
	QList<SourcePreset> sourcePresets;
	for (const QJsonValue& value : jsonArray)
	{
		QJsonObject sourceObj = value.toObject();
		if (!sourceObj.contains("id") || !sourceObj.contains("name") || !sourceObj.contains("plugin_id") || !sourceObj.contains("type") || !sourceObj.contains("config")) {
			qWarning() << "Invalid source preset format"; // TODO: Log better
			continue;
		}
		QByteArray sourceId = QByteArray(sourceObj["id"].toString().toStdString());
		QString sourceName = QByteArray(sourceObj["name"].toString().toStdString());
		QString pluginId = sourceObj["plugin_id"].toString();
		Source::Type sourceType = static_cast<Source::Type>(sourceObj["type"].toInt());
		QJsonObject config = sourceObj["config"].toObject();
		SourcePreset preset{
			sourceId,
			sourceName,
			pluginId,
			sourceType,
			config
		};
		sourcePresets.append(preset);
	}
	return sourcePresets;
}
