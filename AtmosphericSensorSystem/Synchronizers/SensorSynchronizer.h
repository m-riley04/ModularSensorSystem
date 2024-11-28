#pragma once

#include <QObject>
#include <opencv2/opencv.hpp>

class SensorSynchronizer  : public QObject
{
	Q_OBJECT

public:
	SensorSynchronizer(QObject *parent);
	~SensorSynchronizer();

public slots:
	void addCameraFrame(const cv::Mat& frame, const qint64 timestamp);
	void addSensorData(const QVariant& data, const qint64 timestamp);

signals:
	void synchronizedDataReady(const cv::Mat& frame, const QVariant& sensorData, const qint64 timestamp);
};
