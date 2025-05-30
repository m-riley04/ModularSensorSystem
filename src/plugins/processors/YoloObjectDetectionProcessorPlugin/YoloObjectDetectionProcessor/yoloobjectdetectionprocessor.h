#pragma once

#include <QObject>
#include <QImage>
#include <QVideoFrame>
#include "processing/ProcessorBase/processorbase.h"

class ProcessorBase;

class YoloObjectDetectionProcessor : public ProcessorBase
{
	Q_OBJECT

public:
	YoloObjectDetectionProcessor(Device* device, QObject *parent);
	~YoloObjectDetectionProcessor();

    // ProcessorBase API
    void startProcessing() override { mEnabled = true; }
    void stopProcessing() override { mEnabled = false; }

private:
    bool mEnabled = true;
    QImage mPreview;

public slots:
	void processFrame(const QVideoFrame& frame);

signals:
    void objectDetected(Device* device);
};

