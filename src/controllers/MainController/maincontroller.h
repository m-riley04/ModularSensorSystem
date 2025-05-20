#pragma once

#include <QObject>
#include <Controllers/DeviceController/devicecontroller.h>
#include <Controllers/ClipController/clipcontroller.h>
#include <controllers/RecordingController/recordingcontroller.h>
#include <controllers/PluginController/plugincontroller.h>
#include <qcoreapplication.h>
#include <QtMultimedia>

class MainController  : public QObject
{
	Q_OBJECT

public:
	MainController(QObject *parent);
	~MainController();

	RecordingController* recordingController() const { return pRecordingController.get(); }
	DeviceController* deviceController() const { return pDeviceController.get(); }
	ClipController* clipController() const { return pClipController.get(); }

private:
	std::unique_ptr<DeviceController> pDeviceController;
	std::unique_ptr<RecordingController> pRecordingController;
	std::unique_ptr<ClipController> pClipController;
	std::unique_ptr<PluginController> pPluginController;
};
