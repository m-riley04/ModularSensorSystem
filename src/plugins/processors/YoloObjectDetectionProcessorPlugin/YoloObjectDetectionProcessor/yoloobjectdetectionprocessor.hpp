#pragma once

#include <QObject>
#include <QImage>
#include <QVideoFrame>
#include "features/processors/processor.hpp"
#include "interfaces/capability/processors/iobjectdetectioncapable.hpp"
#include "interfaces/capability/general/ipipelineelement.hpp"
#include "yoloprocessorbin.h"

class YoloObjectDetectionProcessor
	: public Processor
    , public IObjectDetectionCapable
    , public IPipelineElement
{
	Q_OBJECT

public:
	YoloObjectDetectionProcessor(const ElementInfo& element, QObject *parent);
	~YoloObjectDetectionProcessor();

    // Processor API
    void startProcessing() override { mEnabled = true; }
    void stopProcessing() override { mEnabled = false; }

    // IPipelineElement API
    GstElement* gstSrcBin() override final { return nullptr; }
    GstElement* gstFilterBin() override final;
    GstElement* gstSinkBin() override final { return nullptr; }

    // IObjectDetectionCapable API
	void onObjectDetected(DetectionInfo detection) override;

private:
    bool mEnabled = true;
	std::unique_ptr<YoloProcessorBin> m_processorBin;

signals:
    void objectDetected(Source* source);
};

