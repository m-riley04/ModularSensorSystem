#include "maincontroller.h"

MainController::MainController(QObject *parent)
	: QObject(parent), pDeviceController(std::make_unique<DeviceController>(this))
{}

MainController::~MainController()
{}
