#pragma once

#include <QObject>
#include <Devices/Device/device.h>

class DeviceController  : public QObject
{
	Q_OBJECT

public:
	DeviceController(QObject *parent);
	~DeviceController();

private:
	QList<Device*> mDevices;

public slots:
	void addDevice(Device* device);
	void removeDevice(Device* device);
	Device* getDevice(int index) const;


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
