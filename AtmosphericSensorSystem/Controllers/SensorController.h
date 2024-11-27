#pragma once

#include <QObject>
#include <QThread>
#include "../Sensors/Sensor.h"
#include "../Sensors/Camera.h"

class SensorController : public QObject
{
	Q_OBJECT

private:
	QList<Sensor*> _sensors;
	QLabel* label;
	QThread* worker;
	

public:
	Camera* camera; /// TODO: Generalize this

	SensorController(QObject*parent);
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
