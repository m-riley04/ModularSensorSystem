#include "maincontroller.h"

MainController::MainController(QObject *parent)
	: QObject(parent)
{
	pPluginController = std::make_unique<PluginController>(QCoreApplication::applicationDirPath() + "/plugins", this);
	pDeviceController = std::make_unique<DeviceController>(pPluginController.get(), this);
	pRecordingController = std::make_unique<RecordingController>(pDeviceController.get(), this);
	pPresetsController = std::make_unique<PresetsController>(QCoreApplication::applicationDirPath() + "/presets", this);
	
}

MainController::~MainController()
{}
