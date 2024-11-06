#include "SensorController.h"

SensorController::SensorController(QObject *parent)
	: QObject(parent)
{}

SensorController::~SensorController()
{}

QList<Sensor*> SensorController::sensors() const
{
	return _sensors;
}

void SensorController::addSensor(Sensor* sensor)
{
	_sensors.append(sensor);
	emit sensorAdded(sensor);
}

void SensorController::removeSensor(Sensor* sensor)
{
	_sensors.removeOne(sensor);
	emit sensorRemoved(sensor);
}

void SensorController::clearSensors()
{
	_sensors.clear();
	emit sensorsCleared();
}

void SensorController::startSensors()
{
	for (Sensor* sensor : _sensors) {
		sensor->start();
	}
}

void SensorController::stopSensors()
{
	for (Sensor* sensor : _sensors) {
		sensor->stop();
	}
}