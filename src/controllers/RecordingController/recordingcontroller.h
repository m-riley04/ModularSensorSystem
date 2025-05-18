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
	RecordingController(DeviceController* deviceController, QObject *parent);
	~RecordingController();

private:
	QPointer<DeviceController> pDeviceController;
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
