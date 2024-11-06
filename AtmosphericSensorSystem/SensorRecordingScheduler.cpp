#include "SensorRecordingScheduler.h"

SensorRecordingScheduler::SensorRecordingScheduler(QObject *parent)
	: QObject(parent)
{}

SensorRecordingScheduler::~SensorRecordingScheduler()
{}


void SensorRecordingScheduler::start()
{
	/// TODO: Do more to synchronize recordings

	// Iterate through each sensor
	for (Sensor* sensor : _sensors) {
		// Start recording for this sensor
		sensor->startRecording();
	}

	emit started();
}

void SensorRecordingScheduler::pause()
{
	/// TODO: Do more to synchronize recordings

	// Iterate through each sensor
	for (Sensor* sensor : _sensors) {
		// Stop recording for this sensor
		sensor->pauseRecording();
	}

	emit paused();
}

void SensorRecordingScheduler::stop()
{
	/// TODO: Do more to synchronize recordings

	// Iterate through each sensor
	for (Sensor* sensor : _sensors) {
		// Stop recording for this sensor
		sensor->stopRecording();
	}

	emit stopped();
}