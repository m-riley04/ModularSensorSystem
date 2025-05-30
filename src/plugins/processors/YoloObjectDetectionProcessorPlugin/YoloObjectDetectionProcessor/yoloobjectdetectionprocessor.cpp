#include "yoloobjectdetectionprocessor.h"

YoloObjectDetectionProcessor::YoloObjectDetectionProcessor(Device* device, QObject *parent)
	: ProcessorBase(device, parent)
{}

YoloObjectDetectionProcessor::~YoloObjectDetectionProcessor()
{}

void YoloObjectDetectionProcessor::processFrame(const QVideoFrame& frame) {

}