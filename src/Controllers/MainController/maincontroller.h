#pragma once

#include <QObject>
#include <Controllers/DeviceController/devicecontroller.h>
#include <Controllers/RecordingSession/recordingsession.h>

class MainController  : public QObject
{
	Q_OBJECT

public:
	MainController(QObject *parent);
	~MainController();

	RecordingSession* recordingSession() const { return pRecordingSession.get(); }
	DeviceController* deviceController() const { return pDeviceController.get(); }

private:
	std::unique_ptr<DeviceController> pDeviceController;
	std::unique_ptr<RecordingSession> pRecordingSession;
};
