#pragma once

#include <QObject>
#include "plugins/device/ideviceplugin.h"
#include "devices/Device/device.h"
#include "VideoDevice/videodevice.h"
#include <QtMultimedia>

class VideoDevicePlugin : public QObject, public IDevicePlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID IDevicePlugin_iid FILE "videodeviceplugin.json")
	Q_INTERFACES(IDevicePlugin)

public:
	QList<DeviceInfo> availableDevices() const override;
	Device* createDevice(const QByteArray& id, QObject* parent) override;
	QString pluginName() const override { return "VideoDevice"; }
	Device::Type deviceType() const override { return Device::Type::CAMERA; }
};
