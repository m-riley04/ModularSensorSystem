#pragma once

#include <QObject>
#include <QThread>
#include <QtMultimedia>
#include "../Sensors/Sensor.h"
#include "../Sensors/Camera.h"
#include "../Synchronizers/SensorSynchronizer.h"
#include "../Processors/FrameProcessor.h"
#include "../Writers/SensorWriter.h"

class SensorController : public QObject
{
	Q_OBJECT

private:
	QList<Sensor*> mSensors;
	QList<Camera*> mCameras;

	FrameProcessor *pFrameProcessor;
	SensorSynchronizer *pSynchronizer;
	SensorWriter *pWriter;
	
	QThread mCameraThread;
	QThread mProcessorThread;
	QThread mWriterThread;

public:
	SensorController(QObject *parent);
	~SensorController();

	QList<Sensor*> sensors() const;
	QList<Camera*> cameras() const;

public slots:
	void addSensor(Sensor* sensor);
	void removeSensor(Sensor* sensor);
	void clearSensors();
	void startSensors();
	void stopSensors();

	void addCamera(QCameraDevice device);
	void removeCamera(Camera* camera);
	void clearCameras();
	void startCameras();
	void stopCameras();

signals:
	void sensorAdded(Sensor *sensor);
	void sensorRemoved(Sensor *sensor);
	void sensorsCleared();
	
	void cameraStarted(Camera *camera);
	void cameraAdded(Camera* camera);
	void cameraRemoved(Camera *camera);
	void camerasCleared();
};
