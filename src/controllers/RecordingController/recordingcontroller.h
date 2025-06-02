#pragma once

#include <QObject>
#include <qelapsedtimer.h>
#include <qdir.h>
#include <devices/Device/device.h>
#include <QDateTime>
#include "controllers/RecordingSession/recordingsession.h"
#include <controllers/DeviceController/devicecontroller.h>

class RecordingController  : public QObject
{
	Q_OBJECT

public:
	enum State {
		STARTED,
		STOPPED
	};

public:
	RecordingController(SourceController* sourceController, QObject *parent);
	~RecordingController();

	RecordingController::State state() const { return mState; }
	bool isRecording() const { return mState == STARTED; }
	bool isStopped() const { return mState == STOPPED; }

private:
	QPointer<SourceController> pDeviceController;
	std::unique_ptr<RecordingSession> pSession;

	RecordingController::State mState = STOPPED;

public slots:
	void start();
	void stop();

private slots:
	void onDeviceAdded(Device* device);
	void onDeviceRemoved(Device* device);

signals:
	void started(RecordingSession* session);
	void stopped(QDir output);
};
