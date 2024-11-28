#include "SensorWriter.h"

SensorWriter::SensorWriter(QObject* parent)
{}

SensorWriter::~SensorWriter()
{}

void SensorWriter::write(const QVariant& sensorData, const qint64 timestamp) {
	
	
	emit writeFinished(timestamp);
}