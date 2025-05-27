#pragma once

#include <QObject>
#include <Controllers/DeviceController/devicecontroller.h>
#include <Controllers/ClipController/clipcontroller.h>
#include <controllers/RecordingController/recordingcontroller.h>
#include <controllers/PluginController/plugincontroller.h>
#include <qcoreapplication.h>
#include <QtMultimedia>
#include <controllers/PresetsController/presetscontroller.h>

class MainController  : public QObject
{
	Q_OBJECT

public:
	MainController(QObject *parent);
	~MainController();

	RecordingController* recordingController() const { return pRecordingController.get(); }
	DeviceController* deviceController() const { return pDeviceController.get(); }
	ClipController* clipController() const { return pClipController.get(); }
	PluginController* pluginController() const { return pPluginController.get(); }
	PresetsController* presetsController() const { return pPresetsController.get(); }

private:
	std::unique_ptr<DeviceController> pDeviceController;
	std::unique_ptr<RecordingController> pRecordingController;
	std::unique_ptr<ClipController> pClipController;
	std::unique_ptr<PluginController> pPluginController;
	std::unique_ptr<PresetsController> pPresetsController;

signals:
	void errorOccurred(const DeviceError& e);
};
