#pragma once

#include <QObject>
#include <QImage>
#include <QVideoFrame>
#include "features/processors/processor.hpp"
#include "interfaces/capability/processors/iobjectdetectioncapable.hpp"
#include "interfaces/capability/general/ipipelineelement.hpp"
#include "yoloprocessorbranch.h"

class YoloObjectDetectionProcessor
	: public Processor
    , public IObjectDetectionCapable
    , public IPipelineElement
{
	Q_OBJECT

public:
	YoloObjectDetectionProcessor(const ElementInfo& element, QObject *parent);
	~YoloObjectDetectionProcessor();

	// Element lifecycle hooks
	void onSessionStart() override;
	void onSessionStop() override;

    // Processor API
    bool startProcessing() override;
    bool stopProcessing() override;

    // IPipelineElement API
    GstElement* gstSrcBin() override final { return nullptr; }
    GstElement* gstFilterBin() override final;
    GstElement* gstSinkBin() override final { return nullptr; }
    ProcessingBranch* processingBranch() override final;

    // IObjectDetectionCapable API
	void onObjectDetected(DetectionInfo detection) override;

private:
	void createBranchIfNeeded();

	std::unique_ptr<YoloProcessorBranch> m_processorBin;

};

