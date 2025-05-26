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
	if (!device) {
		qDebug() << "ClipController::clip: device is null";
		return;
	}

	// Check path
	if (dirPath.isEmpty()) {
		dirPath = mClippingDir;
	}
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