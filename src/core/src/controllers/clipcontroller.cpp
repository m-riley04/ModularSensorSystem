#include "controllers/clipcontroller.h"


ClipController::ClipController(QString clippingDir, QObject *parent)
	: BackendControllerBase("ClipController", parent), mClippingDir(clippingDir)
{}

ClipController::~ClipController()
{}

void ClipController::clip(QList<Source*> sources, QString dirPath)
{
	// Check sources
	if (sources.isEmpty()) {
		qDebug() << "ClipController::clip: no sources provided";
		return;
	}

	// Check path
	if (dirPath.isEmpty()) {
		dirPath = mClippingDir;
	}

	// Iterate through sources and clip each one
	for (Source* source : sources) {
		if (!source) {
			qDebug() << "ClipController::clip: source is null";
			continue;
		}
		clip(source, dirPath);
	}
}

void ClipController::clip(Source* source, QString dirPath)
{
	// Check source
	auto clippableSource = qobject_cast<IClippableSource*>(source);
	if (!clippableSource) {
		qWarning() << "Source is not clippable: aborting clip";
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

	// Clip the source and save to the directory
	clippableSource->clip(clipDir);
	emit clipSaved(clipDir.absolutePath());
}

void ClipController::flush(QList<Source*> sources)
{
	// Check sources
	if (sources.isEmpty()) {
		qDebug() << "Cannot flush buffers: no sources provided";
		return;
	}

	// Iterate through sources and flush each one
	for (Source* source : sources) {
		flush(source);
	}
}

void ClipController::flush(Source* source)
{
	// Check source
	if (!source) {
		qDebug() << "Cannot flush buffer: source is null";
		return;
	}

	// Cast source to IClippableSource
	auto clippableSource = qobject_cast<IClippableSource*>(source);
	if (!clippableSource) {
		qDebug() << "Cannot flush buffer: source is not clippable";
		return;
	}

	// Flush the source's buffer
	clippableSource->clipBuffer()->flush();
	emit flushed(clippableSource->clipBuffer());
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
