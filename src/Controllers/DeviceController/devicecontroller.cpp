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
	emit devicesOpened();
}

void DeviceController::startDevices()
{
	for (Device* device : mDevices) {
		if (device) {
			device->start();
		}
	}
	emit devicesStarted();
}

void DeviceController::stopDevices()
{
	for (Device* device : mDevices) {
		if (device) {
			device->stop();
		}
	}
	emit devicesStopped();
}

void DeviceController::closeDevices()
{
	for (Device* device : mDevices) {
		if (device) {
			device->close();
		}
	}
	emit devicesClosed();
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
	if (device) {
		mDevices.removeAll(device);
		delete device; // CONSIDER: determine where to clean up
	}

	emit deviceRemoved(device);
}