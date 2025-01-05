#pragma once

#include <QObject>
#include <QThread>
#include <QtMultimedia>
#include "../Sensors/Sensor.h"
#include "../Sensors/Camera.h"
#include <memory>
#include <vector>

class SensorController : public QObject
{
	Q_OBJECT

private:
	std::vector<std::unique_ptr<Sensor>> mSensors;
	std::vector<std::unique_ptr<Camera>> mCameras;

public:
	SensorController(QObject *parent);
	~SensorController();

	const std::vector<std::unique_ptr<Sensor>> &sensors() const;
	const std::vector<std::unique_ptr<Camera>> &cameras() const;

public slots:
	void addSensor();
	void removeSensor(Sensor* sensor);
	void clearSensors();
	void startSensors();
	void stopSensors();

	void addCamera(QCameraDevice device);
	void removeCamera(Camera* camera);
	void clearCameras();
	void startCameras();
	void stopCameras();

	Camera* findCamera(QVideoWidget *widget);

signals:
	void sensorAdded(Sensor *sensor);
	void sensorRemoved(Sensor *sensor);
	void sensorsCleared();
	
	void cameraStarted(Camera *camera);
	void cameraAdded(Camera* camera);
	void cameraRemoved(Camera *camera);
	void camerasCleared();
};
