#include "yoloobjectdetectionprocessor.hpp"

YoloObjectDetectionProcessor::YoloObjectDetectionProcessor(const ElementInfo& element, QObject *parent)
	: Processor(element, parent)
	, m_processorBin(std::make_unique<YoloProcessorBin>(this->uuid(), element.id))
{}

YoloObjectDetectionProcessor::~YoloObjectDetectionProcessor()
{}

GstElement* YoloObjectDetectionProcessor::gstFilterBin()
{
	// Lazy initialization
	if (!m_processorBin) {
		std::make_unique<YoloProcessorBin>(this->uuid(), this->id());
	}
	return m_processorBin->bin();
}

void YoloObjectDetectionProcessor::onObjectDetected(DetectionInfo detection)
{

}

void YoloObjectDetectionProcessor::startProcessing()
{
	if (!m_processorBin) return;

	return m_processorBin->setProcessingEnabled(true);
}

void YoloObjectDetectionProcessor::stopProcessing()
{
	if (!m_processorBin) return;
	// Close the valve FIRST, THEN send the EOS to finalize the file
	return m_processorBin->setProcessingEnabled(false);
}