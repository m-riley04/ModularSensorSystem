#include "Sensor.h"


Sensor::Sensor(QObject *parent)
	: QObject(parent)
{}

Sensor::~Sensor()
{}

double Sensor::interval()
{
	return _interval;
}

void Sensor::setInterval(double interval)
{
	if (_interval != interval) {
		_interval = interval;
		emit intervalChanged(interval);
	}
}

void Sensor::setPortName(const QString& portName) {
	_serialPort.setPortName(portName);
}