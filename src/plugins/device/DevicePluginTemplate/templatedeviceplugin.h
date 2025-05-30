#pragma once

#include <QObject>
#include "interfaces/plugins/ideviceplugin.h"
#include "devices/Device/device.h"
#include "TemplateDevice/templatedevice.h"

class TemplateDevicePlugin : public QObject, public IDevicePlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID IDevicePlugin_iid FILE "templatedeviceplugin.json")
	Q_INTERFACES(IDevicePlugin)

public:
	QList<DeviceInfo> availableDevices() const override;
	Device* createDevice(const QByteArray& id, QObject* parent) override;
	QString pluginName() const override { return "TemplateDevice"; }
	Device::Type deviceType() const override { return Device::Type::OTHER; }
};
