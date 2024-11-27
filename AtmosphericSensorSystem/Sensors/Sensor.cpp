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

QString& Sensor::outputPrefix()
{
	return _outputPrefix;
}

double Sensor::interval()
{
	return _interval;
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

void Sensor::setOutputPrefix(QString prefix)
{
	if (_outputPrefix != prefix) {
		_outputPrefix = prefix;
		emit outputPrefixChanged(prefix);
	}
}

void Sensor::setInterval(double interval)
{
	if (_interval != interval) {
		_interval = interval;
		emit intervalChanged(interval);
	}
}

void Sensor::setState(SensorState state)
{
	if (_state != state) {
		_state = state;
		emit stateChanged(state);
	}
}

void Sensor::setPortName(const QString& portName) {
	_serialPort.setPortName(portName);
}