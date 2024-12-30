#include "SensorController.h"
#include "../Windows/MainWindow.h"

SensorController::SensorController(QObject *parent)
	: QObject(parent)
{
	// Initialize components
	pSynchronizer = std::make_unique<SensorSynchronizer>();
	pFrameProcessor = std::make_unique<FrameProcessor>();
	pWriter = std::make_unique<SensorWriter>();

	// Initialize threads
	pFrameProcessor->moveToThread(&mProcessorThread);
	pWriter->moveToThread(&mWriterThread);
	mProcessorThread.start();
	mWriterThread.start();
}

SensorController::~SensorController()
{
	// Clean up threads
	mProcessorThread.quit();
	mProcessorThread.wait();

	mWriterThread.quit();
	mWriterThread.wait();

	// Clean up sensors/cameras
	mCameras.clear();
	mSensors.clear();
}

const std::vector<std::unique_ptr<Sensor>> &SensorController::sensors() const
{
	return mSensors;
}

void SensorController::addSensor()
{
	//mSensors.push_back(sensor);
	//emit sensorAdded(mSensors.back().get());
}

void SensorController::removeSensor(Sensor *sensor)
{
	auto it = std::find_if(mSensors.begin(), mSensors.end(),
		[sensor](const std::unique_ptr<Sensor>& ptr) {
			return ptr.get() == sensor;
		});
	if (it != mSensors.end()) {
		emit sensorRemoved(sensor);
		mSensors.erase(it); // This will automatically delete the Sensor
	}
}

void SensorController::clearSensors()
{
	mSensors.clear();
	emit sensorsCleared();
}

void SensorController::startSensors()
{
	for (const std::unique_ptr<Sensor>& sensor : mSensors) {
		sensor->start();
	}
}

void SensorController::stopSensors()
{
	for (const std::unique_ptr<Sensor>& sensor : mSensors) {
		sensor->stop();
	}
}

const std::vector<std::unique_ptr<Camera>>& SensorController::cameras() const
{
	return mCameras;
}

void SensorController::addCamera(QCameraDevice device)
{
	auto camera = std::make_unique<Camera>();
	camera->setDevice(device);
	camera->start();

	mCameras.push_back(std::move(camera));
	emit cameraAdded(mCameras.back().get());
}

void SensorController::removeCamera(Camera* camera)
{
	auto it = std::find_if(mCameras.begin(), mCameras.end(),
		[camera](const std::unique_ptr<Camera>& ptr) {
			return ptr.get() == camera;
		});
	if (it != mCameras.end()) {
		emit cameraRemoved(camera);
		mCameras.erase(it);
	}
}

void SensorController::clearCameras()
{
	mCameras.clear();
	emit camerasCleared();
}

void SensorController::startCameras()
{
	for (const auto& camera : mCameras) {
		camera->start();
	}
}

void SensorController::stopCameras()
{
	for (const auto& camera : mCameras) {
		camera->stop();
	}
}

Camera* SensorController::findCamera(QVideoWidget* widget)
{
	for (const auto& camera : mCameras) {
		if (camera->session().videoOutput() == widget) return camera.get();
	}
}
