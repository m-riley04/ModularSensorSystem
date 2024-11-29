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
	QList<Sensor*> _sensors;

	Camera* camera; /// TODO: Generalize this
	FrameProcessor* frameProcessor;
	SensorSynchronizer* synchronizer;
	SensorWriter* writer;
	
	QThread* cameraThread;
	QThread* processorThread;
	QThread* writerThread;

	QCamera q_camera;
	QMediaCaptureSession session;
	QMediaRecorder recorder;
	QVideoSink sink;

public:
	SensorController(QObject* parent);
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
