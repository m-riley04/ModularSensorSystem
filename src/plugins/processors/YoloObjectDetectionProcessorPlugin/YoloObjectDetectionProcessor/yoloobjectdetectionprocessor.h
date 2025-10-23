#pragma once

#include <QObject>
#include <QImage>
#include <QVideoFrame>
#include "features/processors/processor.h"

class Processor;

class YoloObjectDetectionProcessor : public Processor
{
	Q_OBJECT

public:
	YoloObjectDetectionProcessor(Source* src, QObject *parent);
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
    void objectDetected(Source* source);
};

