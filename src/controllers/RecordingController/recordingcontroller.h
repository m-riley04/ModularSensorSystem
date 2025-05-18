#pragma once

#include <QObject>
#include <qelapsedtimer.h>
#include <qdir.h>
#include <devices/Device/device.h>
#include <QDateTime>
#include "controllers/RecordingSession/recordingsession.h"

class RecordingController  : public QObject
{
	Q_OBJECT

public:
	RecordingController(QObject *parent);
	~RecordingController();

private:
	std::unique_ptr<RecordingSession> pSession;

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
