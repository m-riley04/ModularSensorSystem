#pragma once

#include <QObject>
#include <devices/Device/device.h>
#include <controllers/DeviceController/devicecontroller.h>
#include <presets/Preset/preset.h>
#include <devices/IConfigurableDevice/iconfigurabledevice.h>
#include <qjsonarray.h>
#include <QDir>
#include <qdiriterator.h>

class PresetsController : public QObject
{
	Q_OBJECT

public:
	PresetsController(const QString& dir, QObject *parent);
	~PresetsController();

	QList<Preset> presets() const { return mPresets; }
	QString presetsDir() const { return mPresetsDir; }

	void savePreset(QString name, QList<Device*> activeDevices = QList<Device*>(), QString dirPath = QString());
	void loadPreset(const QString& path, DeviceController* deviceController);

	void loadAllPresets(DeviceController* deviceController);

	void scanForPresets(QString presetDir = QString());

private:
	QString mPresetsDir;
	QList<Preset> mPresets;

	QJsonArray devicePresetsToJson(const QList<DevicePreset>& devicePresets);

signals:
	void presetSaved(const QString& path);
	void presetLoaded(const QString& path);

};
