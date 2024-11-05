#include "Sensor.h"

Sensor::Sensor(QObject *parent)
	: QObject(parent)
{}

Sensor::~Sensor()
{}

void Sensor::setOutputDirectory(QUrl dir)
{
	if (outputDir != dir) {
		outputDir = dir;
		emit outputDirectoryChanged(dir);
	}
}

QUrl& Sensor::getOutputDirectory()
{
	return outputDir;
}
