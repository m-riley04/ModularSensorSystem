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
			config = icfg->saveSettings();
		}

		DevicePreset preset {
			device->id(),
			device->name(),
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

	emit presetSaved(fullPath);
}

void PresetsController::loadPreset(const QString& path, DeviceController* deviceController, PluginController* pluginController)
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
	if (!obj.contains("name") || !obj.contains("devices")) {
		qWarning() << "Invalid preset format:" << path;
		return;
	}

	QString name = obj["name"].toString();
	QJsonArray devicesArray = obj["devices"].toArray();
	QList<DevicePreset> devicePresets = jsonToDevicePresets(devicesArray);

	// Iterate each device preset and load it into the device controller
	for (const DevicePreset& devicePreset : devicePresets)
	{
		// Clear the current device list
		deviceController->clearDevices();

		// Get the plugin for the device
		IDevicePlugin* plugin = pluginController->getDevicePlugin(devicePreset.pluginId);
		if (!plugin) {
			qWarning() << "Plugin is null: aborting device load";
			return;
		}

		// Create DeviceInfo object for the device
		DeviceInfo deviceInfo;
		deviceInfo.id = devicePreset.deviceId;
		deviceInfo.displayName = devicePreset.deviceName;
		deviceInfo.type = devicePreset.deviceType;

		// Create the device
		Device* device = deviceController->addDevice(plugin, deviceInfo);
		if (!device) {
			qWarning() << "Device is null: aborting device load";
			return;
		}
		
		// Load the preset into the device
		if (auto icfg = qobject_cast<IConfigurableDevice*>(device)) {
			icfg->loadSettings(devicePreset.settings);
		}
		else {
			qWarning() << "Device is not configurable:" << device->id();
			continue;
		}
	}
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
			if (!deviceObj.contains("id") || !deviceObj.contains("name") || !deviceObj.contains("plugin_id") || !deviceObj.contains("type") || !deviceObj.contains("config")) {
				qWarning() << "Invalid device preset format:" << filePath;
				continue;

				QByteArray deviceId = QByteArray(deviceObj["id"].toString().toStdString());
				QString deviceName = deviceObj["device_name"].toString();
				QString pluginId = deviceObj["plugin_id"].toString();
				Device::Type deviceType = static_cast<Device::Type>(deviceObj["type"].toInt());
				QJsonObject config = deviceObj["config"].toObject();
				DevicePreset preset{
					deviceId,
					deviceName,
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
	QJsonArray array = QJsonArray();
	for (auto& devicePreset : devicePresets)
	{
		QJsonObject obj;
		obj["id"] = QString::fromStdString(devicePreset.deviceId.toStdString());
		obj["name"] = devicePreset.deviceName;
		obj["plugin_id"] = devicePreset.pluginId;
		obj["type"] = devicePreset.deviceType;
		obj["config"] = devicePreset.settings;
		array.append(obj);
	}
	return array;
}

QList<DevicePreset> PresetsController::jsonToDevicePresets(const QJsonArray& jsonArray)
{
	QList<DevicePreset> devicePresets;
	for (const QJsonValue& value : jsonArray)
	{
		QJsonObject deviceObj = value.toObject();
		if (!deviceObj.contains("id") || !deviceObj.contains("name") || !deviceObj.contains("plugin_id") || !deviceObj.contains("type") || !deviceObj.contains("config")) {
			qWarning() << "Invalid device preset format"; // TODO: Log better
			continue;
		}
		QByteArray deviceId = QByteArray(deviceObj["id"].toString().toStdString());
		QString deviceName = QByteArray(deviceObj["name"].toString().toStdString());
		QString pluginId = deviceObj["plugin_id"].toString();
		Device::Type deviceType = static_cast<Device::Type>(deviceObj["type"].toInt());
		QJsonObject config = deviceObj["config"].toObject();
		DevicePreset preset{
			deviceId,
			deviceName,
			pluginId,
			deviceType,
			config
		};
		devicePresets.append(preset);
	}
	return devicePresets;
}
