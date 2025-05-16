#include "maincontroller.h"

MainController::MainController(QObject *parent)
	: QObject(parent)
{
	pRecordingSession = std::make_unique<RecordingSession>(this);
	pDeviceController = std::make_unique<DeviceController>(pRecordingSession.get(), this);
}

MainController::~MainController()
{}
