#pragma once

#include <QObject>
#include <Devices/Device/device.h>
#include <Controllers/RecordingSession/recordingsession.h>
#include <qcameradevice.h>

class DeviceController : public QObject
{
	Q_OBJECT

public:
	enum State {
		OPENED,
		CLOSED,
		STARTED,
		STOPPED,
		ERROR
	};

public:
	DeviceController(QObject *parent);
	~DeviceController();

	DeviceController::State state() const { return mState; }
	QList<Device*> devices() const { return mDevices; }

	bool isEmpty() const { return mDevices.isEmpty(); }
	bool isOpen() const { return mState == OPENED; }
	bool isStarted() const { return mState == STARTED; }
	bool isStopped() const { return mState == STOPPED; }
	bool isClosed() const { return mState == CLOSED; }
	bool isError() const { return mState == ERROR; }

private:
	QList<Device*> mDevices;
	DeviceController::State mState = CLOSED;

	RecordingSession* pRecordingSession;

public slots:
	void addDevice(Device* device);
	void removeDevice(Device* device);
	Device* getDevice(QByteArray id) const;

	void openDevices();
	void startDevices();
	void stopDevices();
	void closeDevices();

	void restartDevices();

signals:
	void devicesOpened();
	void devicesStarted();
	void devicesStopped();
	void devicesClosed();

	void devicesRestarted();

	void stateChanged(DeviceController::State state);

	void deviceAdded(Device* device);
	void deviceRemoved(Device* device);
};
