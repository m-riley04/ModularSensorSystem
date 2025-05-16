#pragma once

#include <QObject>
#include <Controllers/DeviceController/devicecontroller.h>
#include <Controllers/RecordingSession/recordingsession.h>
#include <Controllers/ClipController/clipcontroller.h>

class MainController  : public QObject
{
	Q_OBJECT

public:
	MainController(QObject *parent);
	~MainController();

	RecordingSession* recordingSession() const { return pRecordingSession.get(); }
	DeviceController* deviceController() const { return pDeviceController.get(); }
	ClipController* clipController() const { return pClipController.get(); }

private:
	std::unique_ptr<DeviceController> pDeviceController;
	std::unique_ptr<RecordingSession> pRecordingSession;
	std::unique_ptr<ClipController> pClipController;
};
