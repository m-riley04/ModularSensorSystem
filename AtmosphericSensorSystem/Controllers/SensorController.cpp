#include "SensorController.h"
#include "../Windows/MainWindow.h"

SensorController::SensorController(QObject *parent)
	: QObject(parent)
{
	// Initialize components
	p_synchronizer = new SensorSynchronizer();
	p_frameProcessor = new FrameProcessor();
	p_writer = new SensorWriter();

	p_frameProcessor->moveToThread(&m_processorThread);
	p_writer->moveToThread(&m_writerThread);

	// Connect other signals/slots
	MainWindow *mainWindow = qobject_cast<MainWindow*>(parent);
	connect(this, &SensorController::cameraAdded, mainWindow, &MainWindow::addVideoWidget);
	
	// Start threads
	m_processorThread.start();
	m_writerThread.start();

	// Initialize cameras only
	for (QCameraDevice device : QMediaDevices::videoInputs()) {
		Camera* _camera = new Camera();
		_camera->setVideoDevice(device);
		_camera->start();
		addSensor(_camera);
		emit cameraAdded(_camera);
	}
}

SensorController::~SensorController()
{
	// Clean up threads
	m_processorThread.quit();
	m_processorThread.wait();

	m_writerThread.quit();
	m_writerThread.wait();

	// Clean up pointers
	delete p_frameProcessor;
	delete p_synchronizer;
	delete p_writer;
}

QList<Sensor*> SensorController::sensors() const
{
	return r_sensors;
}

void SensorController::addSensor(Sensor *sensor)
{
	r_sensors.append(sensor);
	emit sensorAdded(sensor);
}

void SensorController::removeSensor(Sensor *sensor)
{
	r_sensors.removeOne(sensor);
	emit sensorRemoved(sensor);
}

void SensorController::clearSensors()
{
	r_sensors.clear();
	emit sensorsCleared();
}

void SensorController::startSensors()
{
	for (Sensor* sensor : r_sensors) {
		sensor->start();
	}
}

void SensorController::stopSensors()
{
	for (Sensor* sensor : r_sensors) {
		sensor->stop();
	}
}
