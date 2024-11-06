#include "SensorDataWriter.h"

SensorDataWriter::SensorDataWriter(QObject *parent)
	: QObject(parent)
{}

SensorDataWriter::~SensorDataWriter()
{}

SensorWriterState SensorDataWriter::state() const
{
	return _state;
}