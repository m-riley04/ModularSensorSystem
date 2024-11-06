#include "SensorWriter.h"

SensorWriter::SensorWriter()
{}

SensorWriter::~SensorWriter()
{}

SensorWriterState SensorWriter::state() const
{
	return _state;
}