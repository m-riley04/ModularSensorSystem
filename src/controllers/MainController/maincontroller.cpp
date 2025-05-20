#include "maincontroller.h"

MainController::MainController(QObject *parent)
	: QObject(parent)
{
	pPluginController = std::make_unique<PluginController>(QCoreApplication::applicationDirPath() + "/plugins");
	pDeviceController = std::make_unique<DeviceController>(pPluginController.get(), this);
	pRecordingController = std::make_unique<RecordingController>(pDeviceController.get(), this);
	
}

MainController::~MainController()
{}
