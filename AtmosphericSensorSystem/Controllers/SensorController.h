#pragma once

#include <QObject>
#include "../Sensors/Sensor.h"

class SensorController  : public QObject
{
	Q_OBJECT

private:
	QList<Sensor*> _sensors;

public:
	SensorController(QObject *parent);
	~SensorController();

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
