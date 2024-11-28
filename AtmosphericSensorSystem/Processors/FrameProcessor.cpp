#include "FrameProcessor.h"

FrameProcessor::FrameProcessor(QObject *parent)
	: QObject(parent)
{}

FrameProcessor::~FrameProcessor()
{}

void FrameProcessor::processFrame(const cv::Mat& frame, const qint64 timestamp) {


	emit frameProcessed(frame, timestamp);
}