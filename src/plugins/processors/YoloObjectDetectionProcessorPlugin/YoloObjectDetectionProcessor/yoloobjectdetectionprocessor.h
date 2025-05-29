#pragma once

#include <QObject>
#include <QImage>
#include <QVideoFrame>
#include "processing/ProcessorBase/processorbase.h"

class YoloObjectDetectionProcessor : public ProcessorBase
{
	Q_OBJECT

public:
	YoloObjectDetectionProcessor(Device* device, QObject *parent);
	~YoloObjectDetectionProcessor();

    // ProcessorBase API
    void startProcessing() override { mEnabled = true; }
    void stopProcessing() override { mEnabled = false; }

    // Settings
    void setThreshold(int t) { mThreshold = t; }
    int  threshold()   const { return mThreshold; }

private:
    bool mEnabled = true;
    int  mThreshold = 25;        // simplistic pixel diff threshold
    QImage mPreview;

public slots:
	void processFrame(const QVideoFrame& frame) override;

signals:
    void objectDetected(Device* device);
};

