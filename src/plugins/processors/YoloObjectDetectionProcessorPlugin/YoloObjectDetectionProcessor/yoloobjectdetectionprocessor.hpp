#pragma once

#include <QObject>
#include <QImage>
#include <QVideoFrame>
#include "features/processors/processor.hpp"
#include "interfaces/capability/processors/iobjectdetectioncapable.hpp"
#include "interfaces/capability/general/ipipelinefilter.hpp"
#include "yoloprocessorbin.h"

class YoloObjectDetectionProcessor
	: public Processor
    , public IObjectDetectionCapable
{
	Q_OBJECT

public:
	YoloObjectDetectionProcessor(const ElementInfo& element, QObject *parent);
	~YoloObjectDetectionProcessor();

    // Processor API
    void startProcessing() override { mEnabled = true; }
    void stopProcessing() override { mEnabled = false; }
    GstElement* processorFilterBin() override;

    // IObjectDetectionCapable API
	void onObjectDetected(DetectionInfo detection) override;

private:
    bool mEnabled = true;
	std::unique_ptr<YoloProcessorBin> m_processorBin;

signals:
    void objectDetected(Source* source);
};

