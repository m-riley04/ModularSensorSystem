#include "SensorController.h"
#include "../Windows/MainWindow.h"

SensorController::SensorController(QObject *parent)
	: QObject(parent)
{
	// Initialize components
	pSynchronizer = new SensorSynchronizer();
	pFrameProcessor = new FrameProcessor();
	pWriter = new SensorWriter();

	pFrameProcessor->moveToThread(&mProcessorThread);
	pWriter->moveToThread(&mWriterThread);

	// Connect other signals/slots
	MainWindow *mainWindow = qobject_cast<MainWindow*>(parent);
	connect(this, &SensorController::cameraAdded, mainWindow, &MainWindow::addVideoWidget);
	
	// Start threads
	mProcessorThread.start();
	mWriterThread.start();

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
	mProcessorThread.quit();
	mProcessorThread.wait();

	mWriterThread.quit();
	mWriterThread.wait();

	// Clean up pointers
	delete pFrameProcessor;
	delete pSynchronizer;
	delete pWriter;
}

QList<Sensor*> SensorController::sensors() const
{
	return mSensors;
}

void SensorController::addSensor(Sensor *sensor)
{
	mSensors.append(sensor);
	emit sensorAdded(sensor);
}

void SensorController::removeSensor(Sensor *sensor)
{
	mSensors.removeOne(sensor);
	emit sensorRemoved(sensor);
}

void SensorController::clearSensors()
{
	mSensors.clear();
	emit sensorsCleared();
}

void SensorController::startSensors()
{
	for (Sensor* sensor : mSensors) {
		sensor->start();
	}
}

void SensorController::stopSensors()
{
	for (Sensor* sensor : mSensors) {
		sensor->stop();
	}
}
