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
}

PresetsController::~PresetsController()
{}

void PresetsController::savePreset(const QString & path, QList<Device*>&activeDevices)
{
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

	Preset preset{
		QFileInfo(path).baseName(),
		devicePresets
	};

	mPresets.append(preset);

	QFile file(path);
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
}

void PresetsController::loadPreset(const QString& path, DeviceController* deviceController)
{

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

void PresetsController::scanForPresets()
{

}
