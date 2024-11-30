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
	
	
	// Start threads
	mProcessorThread.start();
	mWriterThread.start();

	//// Initialize cameras only
	//for (QCameraDevice device : QMediaDevices::videoInputs()) {
	//	Camera* _camera = new Camera();
	//	_camera->setDevice(device);
	//	_camera->start();
	//	addCamera(_camera);
	//	emit cameraAdded(_camera);
	//}
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

QList<Camera*> SensorController::cameras() const
{
	return QList<Camera*>();
}

void SensorController::addCamera(QCameraDevice device)
{
	Camera* _camera = new Camera();
	_camera->setDevice(device);
	mCameras.append(_camera);
	_camera->start();
	emit cameraAdded(_camera);
}

void SensorController::removeCamera(Camera* camera)
{
	mCameras.removeOne(camera);
	emit cameraRemoved(camera);
}

void SensorController::clearCameras()
{
	mCameras.clear();
	emit camerasCleared();
}

void SensorController::startCameras()
{
	for (Camera* camera : mCameras) {
		camera->start();
	}
}

void SensorController::stopCameras()
{
	for (Camera* camera : mCameras) {
		camera->stop();
	}
}
