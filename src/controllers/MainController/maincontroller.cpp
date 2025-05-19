#include "maincontroller.h"

MainController::MainController(QObject *parent)
	: QObject(parent)
{
	pDeviceController = std::make_unique<DeviceController>(this);
	pRecordingController = std::make_unique<RecordingController>(pDeviceController.get(), this);
	pPluginController = std::make_unique<PluginManager>(qApp->applicationDirPath() + "/plugins");
}

MainController::~MainController()
{}
