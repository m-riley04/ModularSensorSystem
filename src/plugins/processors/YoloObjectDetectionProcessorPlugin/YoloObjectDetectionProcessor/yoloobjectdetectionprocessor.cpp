#include "yoloobjectdetectionprocessor.hpp"
#include <utils/boost_qt_conversions.hpp>

YoloObjectDetectionProcessor::YoloObjectDetectionProcessor(const ElementInfo& element, QObject *parent)
	: Processor(element, parent)
	, m_processorBin(nullptr)
{
	// Branch/bin is created lazily per session build.
}

YoloObjectDetectionProcessor::~YoloObjectDetectionProcessor()
{}

void YoloObjectDetectionProcessor::onSessionStart()
{
	// no-op; branch/bin is created when the pipeline asks for it
}

void YoloObjectDetectionProcessor::onSessionStop()
{
	// Make processor behave like sources: drop any cached GstBin so we never
	// attempt to re-add a previously-parented bin to a new GstPipeline.
	m_processorBin.reset();
}

void YoloObjectDetectionProcessor::createBranchIfNeeded()
{
	if (m_processorBin)
		return;

	m_processorBin = std::make_unique<YoloProcessorBranch>(this);
	m_processorBin->setDetectionsCallback([this](std::vector<DetectionInfo> detections) {
		QMetaObject::invokeMethod(this, [this, detections = std::move(detections)]() mutable {
			if (!detections.empty()) {
				emit objectsDetected(std::move(detections));

				// TODO: optimize this?
				QStringList labels;
				for (auto& d : detections) {
					labels.push_back(QString(d.label));
				}

				AutomationEvent ev;
				ev.type = "processor.objectsDetected";
				ev.elementId = boostUuidToQUuid(this->uuid());
				ev.payload.insert("count", static_cast<int>(detections.size()));
				ev.payload.insert("labels", labels);
				emit automationEvent(ev);
			}
		}, Qt::QueuedConnection);
	});
}

ProcessingBranch* YoloObjectDetectionProcessor::processingBranch()
{
	createBranchIfNeeded();
	return m_processorBin.get();
}

GstElement* YoloObjectDetectionProcessor::gstFilterBin()
{
	createBranchIfNeeded();
	return m_processorBin ? m_processorBin->bin() : nullptr;
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