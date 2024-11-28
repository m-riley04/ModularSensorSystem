#pragma once

#include <QObject>
#include <opencv2/opencv.hpp>

class FrameProcessor  : public QObject
{
	Q_OBJECT

public:
	FrameProcessor(QObject *parent);
	~FrameProcessor();

public slots:
	void processFrame(const cv::Mat& frame, const qint64 timestamp);

signals:
	void frameProcessed(const cv::Mat& processedFrame, const qint64 timestamp);
};
