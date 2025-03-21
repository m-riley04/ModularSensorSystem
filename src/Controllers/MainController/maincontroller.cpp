#include "maincontroller.h"

MainController::MainController(QObject *parent)
	: QObject(parent)
{
	pSensorController = std::make_unique<SensorController>(this);
}

MainController::~MainController()
{}
