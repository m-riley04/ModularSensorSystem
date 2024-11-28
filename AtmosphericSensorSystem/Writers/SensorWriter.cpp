#include "SensorWriter.h"

SensorWriter::SensorWriter()
{}

SensorWriter::~SensorWriter()
{}

void SensorWriter::write(const cv::Mat& frame, const QVariant& sensorData, const qint64 timestamp) {
	
	
	emit writeFinished(timestamp);
}