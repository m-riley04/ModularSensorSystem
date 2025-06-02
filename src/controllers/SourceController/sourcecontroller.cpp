#include "sourcecontroller.h"

SourceController::SourceController(PluginController* pluginController, QObject *parent)
	: QObject(parent), pPluginController(pluginController), pRecordingSession(nullptr)
{}

SourceController::~SourceController()
{}

void SourceController::openSources()
{
	for (Source* device : mSources) {
		if (device) {
			device->open();
		}
	}

	mState = OPENED;
	emit stateChanged(OPENED);
}

void SourceController::startSources()
{
	for (Source* device : mSources) {
		if (device) {
			device->start();
		}
	}

	mState = STARTED;
	emit stateChanged(STARTED);
}

void SourceController::stopSources()
{
	for (Source* device : mSources) {
		if (device) {
			device->stop();
		}
	}

	mState = STOPPED;
	emit stateChanged(STOPPED);
}

void SourceController::closeSources()
{
	for (Source* device : mSources) {
		if (device) {
			device->close();
		}
	}

	mState = CLOSED;
	emit stateChanged(CLOSED);
}

void SourceController::restartSources()
{
	for (Source* device : mSources) {
		if (device) {
			device->restart();
		}
	}

	emit sourcesRestarted();
}

Source* SourceController::addSource(ISourcePlugin* plugin, SourceInfo info) {
	if (!plugin) {
		emit errorOccurred({
			"Cannot add device: plugin is null",
			nullptr,
			ErrorSeverity::CRITICAL
			});
		qWarning() << "Plugin for device is null";
		return nullptr;
	}
	
	auto device = plugin->createSource(info.id, this);

	mSources.append(device);

	// Connect errors
	connect(device, &Source::errorOccurred, this, &SourceController::errorOccurred);

	emit sourceAdded(device);
	return device;
}

void SourceController::removeSource(Source* device)
{
	if (!device) {
		emit errorOccurred({
			"Cannot remove device: device is null",
			nullptr,
			ErrorSeverity::CRITICAL
			});
		qWarning() << "Cannot remove device: device is null";
		return;
	};

	// Get device id before deleting
	QByteArray deviceId = device->id();

	// Remove device from the list
	mSources.removeAll(device);

	emit sourceRemoved(device); // TODO: Emit the device's ID instead of the device itself
}

Source* SourceController::getSource(QByteArray id) const
{
	for (Source* device : mSources) {
		if (device && device->id() == id) {
			return device;
		}
	}

	return nullptr;
}

void SourceController::clearSources()
{
	for (Source* device : mSources) {
		if (device) {
			device->deleteLater();
			emit sourceRemoved(device);
		}
	}

	mSources.clear();
	mState = CLOSED;
	emit stateChanged(CLOSED);
}
