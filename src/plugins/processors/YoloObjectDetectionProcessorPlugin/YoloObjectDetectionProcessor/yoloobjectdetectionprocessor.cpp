#include "yoloobjectdetectionprocessor.h"

YoloObjectDetectionProcessor::YoloObjectDetectionProcessor(Source* src, QObject *parent)
	: ProcessorBase(src, parent)
{}

YoloObjectDetectionProcessor::~YoloObjectDetectionProcessor()
{}

void YoloObjectDetectionProcessor::processFrame(const QVideoFrame& frame) {

}