#pragma once

#include <QObject>
#include <Devices/Device/device.h>
#include <Controllers/RecordingSession/recordingsession.h>
#include <qcameradevice.h>
#include <controllers/PluginController/plugincontroller.h>
#include <devices/Device/DeviceError/deviceerror.h>

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
	DeviceController(PluginController* pluginController, QObject *parent);
	~DeviceController();

	DeviceController::State state() const { return mState; }
	QList<Device*> devices() const { return mDevices; }

	PluginController* pluginController() const { return pPluginController; }

	bool isEmpty() const { return mDevices.isEmpty(); }
	bool isOpen() const { return mState == OPENED; }
	bool isStarted() const { return mState == STARTED; }
	bool isStopped() const { return mState == STOPPED; }
	bool isClosed() const { return mState == CLOSED; }
	bool isError() const { return mState == ERROR; }

private:
	QList<Device*> mDevices;
	DeviceController::State mState = CLOSED;

	QPointer<PluginController> pPluginController;
	RecordingSession* pRecordingSession;

public slots:
	Device* addDevice(IDevicePlugin* plugin, DeviceInfo info);
	void removeDevice(Device* device);
	Device* getDevice(QByteArray id) const;

	void clearDevices();

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

	void errorOccurred(const DeviceError& e);
};
