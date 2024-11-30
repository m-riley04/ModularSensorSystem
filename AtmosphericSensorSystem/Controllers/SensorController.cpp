#include "SensorController.h"
#include "../Windows/MainWindow.h"

SensorController::SensorController(QObject *parent)
	: QObject(parent)
{
	// Initialize components
	synchronizer = new SensorSynchronizer();
	frameProcessor = new FrameProcessor();
	writer = new SensorWriter();

	frameProcessor->moveToThread(&processorThread);
	writer->moveToThread(&writerThread);

	// Connect other signals/slots
	MainWindow* mainWindow = qobject_cast<MainWindow*>(parent);
	connect(this, &SensorController::cameraAdded, mainWindow, &MainWindow::addVideoWidget);
	
	// Start threads
	processorThread.start();
	writerThread.start();

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
	processorThread.quit();
	processorThread.wait();

	writerThread.quit();
	writerThread.wait();

	// Clean up pointers
	delete frameProcessor;
	delete synchronizer;
	delete writer;
}

QList<Sensor*> SensorController::sensors() const
{
	return _sensors;
}

void SensorController::addSensor(Sensor* sensor)
{
	_sensors.append(sensor);
	emit sensorAdded(sensor);
}

void SensorController::removeSensor(Sensor* sensor)
{
	_sensors.removeOne(sensor);
	emit sensorRemoved(sensor);
}

void SensorController::clearSensors()
{
	_sensors.clear();
	emit sensorsCleared();
}

void SensorController::startSensors()
{
	for (Sensor* sensor : _sensors) {
		sensor->start();
	}
}

void SensorController::stopSensors()
{
	for (Sensor* sensor : _sensors) {
		sensor->stop();
	}
}
