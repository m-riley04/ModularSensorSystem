#include "Sensor.h"

Sensor::Sensor(QObject *parent)
	: QObject(parent)
{}

Sensor::~Sensor()
{}

QUrl& Sensor::outputDirectory()
{
	return _outputDir;
}

double Sensor::readInterval()
{
	return _readInterval;
}

SensorState Sensor::state()
{
	return _state;
}

void Sensor::setOutputDirectory(QUrl dir)
{
	if (_outputDir != dir) {
		_outputDir = dir;
		emit outputDirectoryChanged(dir);
	}
}

void Sensor::setReadInterval(double interval)
{
	if (_readInterval != interval) {
		_readInterval = interval;
		emit readIntervalChanged(interval);
	}
}

void Sensor::setState(SensorState state)
{
	if (_state != state) {
		_state = state;
		emit stateChanged(state);
	}
}
