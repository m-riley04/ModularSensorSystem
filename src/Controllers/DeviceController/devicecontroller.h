#pragma once

#include <QObject>
#include <Devices/Device/device.h>
#include <Controllers/RecordingSession/recordingsession.h>
#include "Devices/CameraDevice/cameradevice.h"

class DeviceController : public QObject
{
	Q_OBJECT

public:
	DeviceController(RecordingSession* recordingSession, QObject *parent);
	~DeviceController();

private:
	QList<Device*> mDevices;
	RecordingSession* pRecordingSession;

public slots:
	void addDevice(Device* device);
	void removeDevice(Device* device);
	//Device* getDevice(int index) const;

	void addCamera(const QCameraDevice& cameraDevice);

	void openDevices();
	void startDevices();
	void stopDevices();
	void closeDevices();

signals:
	void devicesOpened();
	void devicesStarted();
	void devicesStopped();
	void devicesClosed();

	void deviceAdded(Device* device);
	void deviceRemoved(Device* device);
};
