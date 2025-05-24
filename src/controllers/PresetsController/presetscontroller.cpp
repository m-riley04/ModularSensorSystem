#include "presetscontroller.h"

PresetsController::PresetsController(const QString& dir, QObject *parent)
	: QObject(parent), mPresetsDir(dir)
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

void PresetsController::savePreset(QString name, QList<Device*> activeDevices, QString dirPath)
{
	// Check dirPath
	if (dirPath.isEmpty()) {
		dirPath = mPresetsDir;
	}

	QList<DevicePreset> devicePresets;
	// Iterate each device
	for (auto device : activeDevices)
	{
		QJsonObject config;
		if (auto icfg = qobject_cast<IConfigurableDevice*>(device)) {
			icfg->saveSettings(config);
		}

		DevicePreset preset {
			device->id(),
			device->pluginId(),
			device->deviceType(),
			config
		};

		devicePresets.append(preset);
	}

	QString fullPath = dirPath + "/" + name + ".json";
	Preset preset{
		name,
		fullPath,
		devicePresets
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
	obj.insert("devices", devicePresetsToJson(devicePresets));
	doc.setObject(obj);

	file.write(doc.toJson());

	// Append the preset if the writing finished successfully
	mPresets.append(preset);
}

void PresetsController::loadPreset(const QString& path, DeviceController* deviceController)
{

}

void PresetsController::loadAllPresets(DeviceController* deviceController)
{
	QDir dir(mPresetsDir);
	for (QString& file : dir.entryList(QDir::Files))
	{
		QString filePath = dir.absoluteFilePath(file);
		
		// Load preset
	}
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
		if (!obj.contains("name") || !obj.contains("devices")) {
			qWarning() << "Invalid preset format:" << filePath;
			continue;
		}
		QString name = obj["name"].toString();
		QJsonArray devicesArray = obj["devices"].toArray();
		QList<DevicePreset> devicePresets;
		for (const QJsonValue& value : devicesArray)
		{
			QJsonObject deviceObj = value.toObject();
			if (!deviceObj.contains("deviceId") || !deviceObj.contains("pluginId") || !deviceObj.contains("deviceType") || !deviceObj.contains("config")) {
				qWarning() << "Invalid device preset format:" << filePath;
				continue;

				QByteArray deviceId = QByteArray(deviceObj["deviceId"].toString().toStdString());
				QString pluginId = deviceObj["pluginId"].toString();
				Device::Type deviceType = static_cast<Device::Type>(deviceObj["deviceType"].toInt());
				QJsonObject config = deviceObj["config"].toObject();
				DevicePreset preset{
					deviceId,
					pluginId,
					deviceType,
					config
				};
				devicePresets.append(preset);
			}
		}

		Preset preset{
			name,
			filePath,
			devicePresets
		};

		// Finally, add the preset to the list
		mPresets.append(preset);
	}
}

QJsonArray PresetsController::devicePresetsToJson(const QList<DevicePreset>& devicePresets)
{
	QJsonArray array;
	for (auto& devicePreset : devicePresets)
	{
		QJsonObject obj;
		obj.insert("deviceId", QString::fromStdString(devicePreset.deviceId.toStdString()));
		obj.insert("pluginId", devicePreset.pluginId);
		obj.insert("deviceType", devicePreset.deviceType);
		obj.insert("config", devicePreset.settings);
		array.append(obj);
	}
	return QJsonArray();
}
