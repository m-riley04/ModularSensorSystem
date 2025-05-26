#include "clipcontroller.h"


ClipController::ClipController(QString clippingDir, QObject *parent)
	: QObject(parent), mClippingDir(clippingDir)
{}

ClipController::~ClipController()
{}

void ClipController::clip(QList<Device*> devices, QString dirPath)
{
	// Check devices
	if (devices.isEmpty()) {
		qDebug() << "ClipController::clip: no devices provided";
		return;
	}

	// Check path
	if (dirPath.isEmpty()) {
		dirPath = mClippingDir;
	}

	// Iterate through devices and clip each one
	for (Device* device : devices) {
		if (!device) {
			qDebug() << "ClipController::clip: device is null";
			continue;
		}
		clip(device, dirPath);
	}
}

void ClipController::clip(Device* device, QString dirPath)
{
	// Check device
	auto clippableDevice = qobject_cast<IClippableDevice*>(device);
	if (!clippableDevice) {
		qWarning() << "Device is not clippable: aborting clip";
		return;
	}

	// Check path
	if (dirPath.isEmpty()) {
		dirPath = mClippingDir;
	}

	// Make timestamped directory
	QString stamp = QDateTime::currentDateTime().toString("'Clip_'yyyyMMdd_HHmmss");
	QDir clipDir(dirPath);
	clipDir.mkpath(stamp);
	clipDir.cd(stamp);

	// Clip the device and save to the directory
	clippableDevice->clip(clipDir);
	emit clipSaved(clipDir.absolutePath());
}

void ClipController::flush(QList<Device*> devices)
{
	// Check devices
	if (devices.isEmpty()) {
		qDebug() << "Cannot flush buffers: no devices provided";
		return;
	}

	// Iterate through devices and flush each one
	for (Device* device : devices) {
		flush(device);
	}
}

void ClipController::flush(Device* device)
{
	// Check device
	if (!device) {
		qDebug() << "Cannot flush buffer: device is null";
		return;
	}

	// Cast device to IClippableDevice
	auto clippableDevice = qobject_cast<IClippableDevice*>(device);
	if (!clippableDevice) {
		qDebug() << "Cannot flush buffer: device is not clippable";
		return;
	}

	// Flush the device's buffer
	clippableDevice->clipBuffer()->flush();
	emit flushed(clippableDevice->clipBuffer());
}

void ClipController::addClipBuffer(ClipBufferBase* buffer)
{
	if (!buffer) {
		qDebug() << "ClipController::addClipBuffer: buffer is null";
		return;
	}

	// Check if the buffer is already in the list
	if (mClipBuffers.contains(buffer)) {
		qDebug() << "ClipController::addClipBuffer: buffer already exists";
		return;
	}

	// Add the buffer to the list
	mClipBuffers.append(buffer);
	emit clipBufferAdded(buffer);
}

void ClipController::removeClipBuffer(ClipBufferBase* buffer)
{
	if (!buffer) {
		qDebug() << "ClipController::removeClipBuffer: buffer is null";
		return;
	}

	// Check if the buffer is in the list
	if (!mClipBuffers.contains(buffer)) {
		qDebug() << "ClipController::removeClipBuffer: buffer not found";
		return;
	}

	// Remove the buffer from the list
	mClipBuffers.removeAll(buffer);
	emit clipBufferRemoved(buffer); // CONSIDER: make this not a pointer?
}
