#include "yoloobjectdetectionprocessor.hpp"

YoloObjectDetectionProcessor::YoloObjectDetectionProcessor(const ElementInfo& element, Source* src, QObject *parent)
	: Processor(element, src, parent)
{}

YoloObjectDetectionProcessor::~YoloObjectDetectionProcessor()
{}

void YoloObjectDetectionProcessor::processFrame(const QVideoFrame& frame) {

}