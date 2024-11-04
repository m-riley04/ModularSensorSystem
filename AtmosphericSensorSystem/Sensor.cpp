#include "Sensor.h"

Sensor::Sensor(QObject *parent)
	: QObject(parent)
{}

Sensor::~Sensor()
{}

void Sensor::setOutputDirectory(QUrl dir)
{
	outputDir = dir;
}

QUrl& Sensor::getOutputDirectory()
{
	return outputDir;
}
