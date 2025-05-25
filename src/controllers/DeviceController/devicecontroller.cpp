#include "devicecontroller.h"

DeviceController::DeviceController(PluginController* pluginController, QObject *parent)
	: QObject(parent), pPluginController(pluginController)
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

Device* DeviceController::addDevice(IDevicePlugin* plugin, DeviceInfo info) {
	if (!plugin) {
		qWarning() << "Plugin for device is null";
		return nullptr;
	}
	
	auto device = plugin->createDevice(info.id, this);

	mDevices.append(device);

	emit deviceAdded(device);
	return device;
}

void DeviceController::removeDevice(Device* device)
{
	if (!device) return;

	// Get device id before deleting
	QByteArray deviceId = device->id();

	// Remove device from the list
	mDevices.removeAll(device);

	emit deviceRemoved(device); // TODO: Emit the device's ID instead of the device itself
}

Device* DeviceController::getDevice(QByteArray id) const
{
	for (Device* device : mDevices) {
		if (device && device->id() == id) {
			return device;
		}
	}

	return nullptr;
}

void DeviceController::clearDevices()
{
	for (Device* device : mDevices) {
		if (device) {
			device->deleteLater();
		}
	}

	mDevices.clear();
	mState = CLOSED;
	emit stateChanged(CLOSED);
}
