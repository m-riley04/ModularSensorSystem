#pragma once

#include <QObject>
#include "Devices/Device/device.h"
#include <qelapsedtimer.h>
#include "Clipping/Buffers/ClipBufferBase/clipbufferbase.h"
#include <qdir.h>
#include <QtMultimedia/qmediarecorder.h>

class Device;

class RecordingSession  : public QObject
{
	Q_OBJECT

	enum State {
		IDLE,
		RECORDING
	};

	enum Error {
		NONE,
		RECORDER_ERROR,
		DEVICE_ERROR,
		UNKNOWN_ERROR
	};

public:
	RecordingSession(QObject *parent);
	~RecordingSession();

private:
	QList<Device*> mDevices;
	QElapsedTimer mTimer;
	QMediaRecorder* mMasterRecorder;
	//Preset mPreset;
	QDir mOutputDir;
	QList<ClipBufferBase*> mClipBuffers;
	RecordingSession::Error error;

};
