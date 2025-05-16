#include "devicecontroller.h"

DeviceController::DeviceController(RecordingSession* recordingSession, QObject *parent)
	: QObject(parent), pRecordingSession(recordingSession)
{}

DeviceController::~DeviceController()
{}

void DeviceController::openDevices()
{
	for (Device* device : mDevices) {
		if (device) {
			device->open();
		}
	}

	mState = OPENED;
	emit stateChanged(OPENED);
}

void DeviceController::startDevices()
{
	for (Device* device : mDevices) {
		if (device) {
			device->start();
		}
	}

	mState = STARTED;
	emit stateChanged(STARTED);
}

void DeviceController::stopDevices()
{
	for (Device* device : mDevices) {
		if (device) {
			device->stop();
		}
	}

	mState = STOPPED;
	emit stateChanged(STOPPED);
}

void DeviceController::closeDevices()
{
	for (Device* device : mDevices) {
		if (device) {
			device->close();
		}
	}

	mState = CLOSED;
	emit stateChanged(CLOSED);
}

void DeviceController::restartDevices()
{
	for (Device* device : mDevices) {
		if (device) {
			device->restart();
		}
	}

	emit devicesRestarted();
}

void DeviceController::addCamera(const QCameraDevice& cameraDevice) {
	auto cam = new CameraDevice(cameraDevice, pRecordingSession, this);

	mDevices.append(cam);

	emit deviceAdded(cam);
}

void DeviceController::addDevice(Device* device) {
	if (device) {
		mDevices.append(device);

		emit deviceAdded(device);
	}
}

void DeviceController::removeDevice(Device* device)
{
	if (!device) return;

	// Get device id before deleting
	QUuid deviceId = device->id();

	// Remove device from the list
	mDevices.removeAll(device);

	emit deviceRemoved(device); // TODO: Emit the device's ID instead of the device itself
}