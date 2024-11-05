#include "SensorRecordingScheduler.h"

SensorRecordingScheduler::SensorRecordingScheduler(QObject *parent)
	: QObject(parent)
{}

SensorRecordingScheduler::~SensorRecordingScheduler()
{}


void SensorRecordingScheduler::start()
{
	emit started();
}

void SensorRecordingScheduler::pause()
{
	emit paused();
}

void SensorRecordingScheduler::stop()
{
	emit stopped();
}