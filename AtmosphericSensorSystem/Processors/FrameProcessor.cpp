#include "FrameProcessor.h"

FrameProcessor::FrameProcessor(QObject *parent)
	: QObject(parent)
{}

FrameProcessor::~FrameProcessor()
{}

void FrameProcessor::processFrame(const QVariant& frame, const qint64 timestamp) {

	// TODO: Process the frame with YOLO

	emit frameProcessed(frame, timestamp);
}