#include "SensorManager.h"

SensorManager::SensorManager(QObject *parent)
	: QObject(parent)
{}

SensorManager::~SensorManager()
{}

QList<Sensor*> SensorManager::sensors() const
{
	return _sensors;
}

void SensorManager::addSensor(Sensor* sensor)
{
	_sensors.append(sensor);
	emit sensorAdded(sensor);
}

void SensorManager::removeSensor(Sensor* sensor)
{
	_sensors.removeOne(sensor);
	emit sensorRemoved(sensor);
}

void SensorManager::clearSensors()
{
	_sensors.clear();
	emit sensorsCleared();
}

void SensorManager::startSensors()
{
	for (Sensor* sensor : _sensors) {
		sensor->start();
	}
}

void SensorManager::stopSensors()
{
	for (Sensor* sensor : _sensors) {
		sensor->stop();
	}
}