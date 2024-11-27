#include "MainController.h"

MainController::MainController(QObject *parent)
	: QObject(parent)
{
    sensorController = new SensorController(this);
}

MainController::~MainController()
{
    sensorController->stopSensors();
    delete sensorController;
}

void MainController::startSensors()
{
	sensorController->startSensors();
}

void MainController::stopSensors()
{
    sensorController->stopSensors();
}