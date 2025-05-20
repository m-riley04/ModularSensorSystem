#pragma once

#include "devices/Device/device.h"

struct DeviceInfo {
	QString id;
	QString displayName;
	Device::Type type;
};

class IDevicePlugin
{

public:
	virtual ~IDevicePlugin() = default;

	/// Enumerate hardware this plugin can drive
	virtual QList<DeviceInfo> availableDevices() const = 0;
	virtual QString pluginName() const = 0;
	virtual Device::Type deviceType() const = 0;
	virtual Device* createDevice(const QByteArray& id, QObject* parent) = 0;
};

#define IDevicePlugin_iid "com.rileymeyerkorth.ModularSensorSystem.IDevicePlugin/1.0"
Q_DECLARE_INTERFACE(IDevicePlugin, IDevicePlugin_iid)