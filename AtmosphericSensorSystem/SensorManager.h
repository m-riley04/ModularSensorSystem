#pragma once

#include <QObject>
#include "Sensor.h"

class SensorManager  : public QObject
{
	Q_OBJECT

private:
	QList<Sensor*> _sensors;

public:
	SensorManager(QObject *parent);
	~SensorManager();

	QList<Sensor*> sensors() const;
	void addSensor(Sensor* sensor);
	void removeSensor(Sensor* sensor);
	void clearSensors();

	void startSensors();
	void stopSensors();

signals:
	void sensorAdded(Sensor* sensor);
	void sensorRemoved(Sensor* sensor);
	void sensorsCleared();
};
