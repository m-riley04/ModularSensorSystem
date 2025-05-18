#include "maincontroller.h"
#include <controllers/RecordingController/recordingcontroller.h>

MainController::MainController(QObject *parent)
	: QObject(parent)
{
	pDeviceController = std::make_unique<DeviceController>(this);
	pRecordingController = std::make_unique<RecordingController>(pDeviceController.get(), this);
	
}

MainController::~MainController()
{}
