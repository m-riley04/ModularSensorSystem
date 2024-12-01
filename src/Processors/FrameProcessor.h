#pragma once

#include <QObject>
#include <QVariant>
#include <opencv2/opencv.hpp>

class FrameProcessor : public QObject
{
	Q_OBJECT

public:
	FrameProcessor(QObject *parent = nullptr);
	~FrameProcessor();

public slots:
	void processFrame(const QVariant& frame, const qint64 timestamp);

signals:
	void frameProcessed(const QVariant& processedFrame, const qint64 timestamp);
};
