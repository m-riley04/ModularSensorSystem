#include "SensorController.h"
#include "Windows/MainWindow/MainWindow.h"

SensorController::SensorController(QObject *parent)
	: QObject(parent)
{}

SensorController::~SensorController()
{
	// Check if recording
	for (const auto& camera : mCameras) {
		if (camera->recorder()->recorderState() == QMediaRecorder::RecorderState::RecordingState) {
			camera->recorder()->stop();
		}
	}

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

QList<QCameraDevice> SensorController::videoDevices()
{
	QList<QCameraDevice> devices = {};
	for (int i = 0; i < mCameras.size(); i++) {
		devices.append(mCameras[i]->camera()->cameraDevice());
	}
	return devices;
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

void SensorController::recordCameras()
{
	for (const auto& camera : mCameras) {
		if (camera->recorder()->recorderState() == QMediaRecorder::RecorderState::StoppedState) camera->recorder()->record();
		else camera->recorder()->stop();
	}
}

Camera* SensorController::findCamera(QVideoWidget* widget)
{
	for (const auto& camera : mCameras) {
		if (camera->session()->videoOutput() == widget) return camera.get();
	}
}

Camera* SensorController::findCamera(CustomSinkWidget* widget)
{
	for (const auto& camera : mCameras) {
		if (camera->session()->videoSink() == widget->videoSink()) return camera.get();
	}
}
