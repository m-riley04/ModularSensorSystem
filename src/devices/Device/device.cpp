#include "device.h"

Device::Device(RecordingSession* recordingSession, QObject *parent)
	: QObject(parent), pRecordingSession(recordingSession)
{}

Device::~Device()
{}
