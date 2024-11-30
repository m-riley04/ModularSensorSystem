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
	QList<Sensor*> r_sensors;
	QList<Camera*> r_cameras;

	FrameProcessor *p_frameProcessor;
	SensorSynchronizer *p_synchronizer;
	SensorWriter *p_writer;
	
	QThread m_cameraThread;
	QThread m_processorThread;
	QThread m_writerThread;

public:
	SensorController(QObject *parent);
	~SensorController();

	QList<Sensor*> sensors() const;
	void addSensor(Sensor *sensor);
	void removeSensor(Sensor *sensor);
	void clearSensors();

	void startSensors();
	void stopSensors();

signals:
	void sensorAdded(Sensor *sensor);
	void sensorRemoved(Sensor *sensor);
	void sensorsCleared();
	
	void cameraStarted(Camera *camera);
	void cameraAdded(Camera *camera);
};
