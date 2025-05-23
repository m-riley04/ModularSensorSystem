#include "recordingcontroller.h"

RecordingController::RecordingController(DeviceController* deviceController, QObject *parent)
	: QObject(parent), pDeviceController(deviceController)
{
	connect(deviceController, &DeviceController::deviceAdded, this, &RecordingController::onDeviceAdded);
	connect(deviceController, &DeviceController::deviceRemoved, this, &RecordingController::onDeviceRemoved);
}

RecordingController::~RecordingController()
{}

void RecordingController::start()
{
	// Init session
	QString rootOutputDirName = "output";
	auto dirName = QDateTime::currentDateTime().toString("'Session_'yyyyMMdd_hhmmss");
	pSession.reset(new RecordingSession(QDir::currentPath() + "/" + rootOutputDirName + "/" + dirName, this));

	for (Device* d : pDeviceController->devices()) {
		d->beginRecording(pSession.get());
	}

	// Update state
	mState = STARTED;
	emit started(pSession.get()); // devices grab the pointer
}

void RecordingController::stop()
{
	//for (auto d : mDevices)  d->stop(); // Probably do not need (starting/stopping is performed by DeviceController)
	QDir outputSessionDir = pSession->outputDir();

	// Stop all devices
	for (Device* d : pDeviceController->devices()) {
		d->endRecording();
	}
	
	// Reset session
	pSession.reset();

	// Update state
	mState = STOPPED;
	emit stopped(outputSessionDir);
}

void RecordingController::onDeviceAdded(Device* device)
{
}

void RecordingController::onDeviceRemoved(Device* device)
{
}


