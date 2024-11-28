#include "SensorSynchronizer.h"

SensorSynchronizer::SensorSynchronizer(QObject *parent)
	: QObject(parent)
{}

SensorSynchronizer::~SensorSynchronizer()
{}

void SensorSynchronizer::addCameraFrame(const cv::Mat& frame, const qint64 timestamp)
{

}

void SensorSynchronizer::addSensorData(const QVariant & data, const qint64 timestamp)
{

}
