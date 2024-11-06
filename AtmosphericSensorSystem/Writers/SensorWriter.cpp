#include "SensorWriter.h"

SensorWriter::SensorWriter(QObject *parent)
	: QObject(parent)
{}

SensorWriter::~SensorWriter()
{}

SensorWriterState SensorWriter::state() const
{
	return _state;
}