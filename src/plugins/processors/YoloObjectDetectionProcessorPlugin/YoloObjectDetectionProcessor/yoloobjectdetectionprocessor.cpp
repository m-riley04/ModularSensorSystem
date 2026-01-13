#include "yoloobjectdetectionprocessor.hpp"

YoloObjectDetectionProcessor::YoloObjectDetectionProcessor(const ElementInfo& element, QObject *parent)
	: Processor(element, parent)
	, m_processorBin(std::make_unique<YoloProcessorBranch>(this))
{}

YoloObjectDetectionProcessor::~YoloObjectDetectionProcessor()
{}

GstElement* YoloObjectDetectionProcessor::gstFilterBin()
{
	// Lazy initialization
	if (!m_processorBin) {
		m_processorBin = std::make_unique<YoloProcessorBranch>(this);
	}
	return m_processorBin->bin();
}

void YoloObjectDetectionProcessor::onObjectDetected(DetectionInfo detection)
{

}

bool YoloObjectDetectionProcessor::startProcessing()
{
	if (!m_processorBin) return false;

	return m_processorBin->setProcessingEnabled(true);
}

bool YoloObjectDetectionProcessor::stopProcessing()
{
	if (!m_processorBin) return false;
	return m_processorBin->setProcessingEnabled(false);
}