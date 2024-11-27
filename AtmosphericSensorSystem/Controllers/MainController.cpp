#include "MainController.h"

MainController::MainController(QObject *parent, QLabel* label)
	: QObject(parent)
{
    sensorController = new SensorController(this, label);
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
