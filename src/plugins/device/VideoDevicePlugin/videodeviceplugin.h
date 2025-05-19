#pragma once

#include <QObject>
#include <plugins/device/IDevicePlugin/ideviceplugin.h>
#include <devices/VideoDevice/videodevice.h>

class VideoDevicePlugin  : public QObject, public IDevicePlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID IDevicePlugin_iid FILE "videodeviceplugin.json")
	Q_INTERFACES(IDevicePlugin)

public:
	QList<DeviceInfo> enumerate() const override;
	Device* create(const QString& id, QObject* parent) override;
};
