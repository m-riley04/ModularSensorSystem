#pragma once

#include <QObject>
#include <opencv2/opencv.hpp>

class SensorWriter : public QObject
{
	Q_OBJECT

public:
	SensorWriter();
	~SensorWriter();

public slots:
	void write(const cv::Mat& frame, const QVariant& sensorData, const qint64 timestamp);

signals:
	void writeFinished(const qint64 timestamp);
};
