#pragma once

#include <devices/Device/device.h>

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
	virtual QList<DeviceInfo> enumerate() const = 0;

	/// Create the actual Device*  (caller takes ownership / parent it)
	virtual Device* create(const QString& id, QObject* parent) = 0;
};

#define IDevicePlugin_iid "com.example.ModularSensorSystem.IDevicePlugin/1.0"
Q_DECLARE_INTERFACE(IDevicePlugin, IDevicePlugin_iid)