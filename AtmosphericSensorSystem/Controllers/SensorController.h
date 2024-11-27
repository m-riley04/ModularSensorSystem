#pragma once

#include <QObject>
#include "../Sensors/Sensor.h"
#include <QLabel>
#include <opencv2/opencv.hpp>
#include "../Sensors/Camera.h"
#include <QThread>

class SensorController  : public QObject
{
	Q_OBJECT

private:
	QList<Sensor*> _sensors;
	QLabel* label;
	QThread* worker;
	

public:
	Camera* camera; /// TODO: Generalize this

	SensorController(QObject *parent, QLabel* label);
	~SensorController();

	QList<Sensor*> sensors() const;
	void addSensor(Sensor* sensor);
	void removeSensor(Sensor* sensor);
	void clearSensors();

	void startSensors();
	void stopSensors();

	

public slots:
	void display(const cv::Mat& frame);

signals:
	void sensorAdded(Sensor* sensor);
	void sensorRemoved(Sensor* sensor);
	void sensorsCleared();
};
