#include "yoloobjectdetectionprocessor.hpp"

YoloObjectDetectionProcessor::YoloObjectDetectionProcessor(const ElementInfo& element, QObject *parent)
	: Processor(element, parent)
	, m_processorBin(std::make_unique<YoloProcessorBranch>(this))
{
	if (m_processorBin) {
		m_processorBin->setDetectionsCallback([this](std::vector<DetectionInfo> detections) {
			// Callback runs on a GStreamer streaming thread; bounce to Qt thread.
			QMetaObject::invokeMethod(this, [this, detections = std::move(detections)]() mutable {
				if (!detections.empty()) {
					emit objectsDetected(std::move(detections));
				}
			}, Qt::QueuedConnection);
		});
	}
}

YoloObjectDetectionProcessor::~YoloObjectDetectionProcessor()
{}

GstElement* YoloObjectDetectionProcessor::gstFilterBin()
{
	// Lazy initialization
	if (!m_processorBin) {
		m_processorBin = std::make_unique<YoloProcessorBranch>(this);
		m_processorBin->setDetectionsCallback([this](std::vector<DetectionInfo> detections) {
			QMetaObject::invokeMethod(this, [this, detections = std::move(detections)]() mutable {
				if (!detections.empty()) {
					emit objectsDetected(std::move(detections));
				}
			}, Qt::QueuedConnection);
		});
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