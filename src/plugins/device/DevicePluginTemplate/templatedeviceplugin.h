#pragma once

#include <QObject>
#include "interfaces/plugins/ideviceplugin.h"
#include "devices/Device/device.h"
#include "TemplateDevice/templatedevice.h"

class TemplateDevicePlugin : public QObject, public ISourcePlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID IDevicePlugin_iid FILE "templatedeviceplugin.json")
	Q_INTERFACES(ISourcePlugin)

public:
	QList<SourceInfo> availableSources() const override;
	Device* createSource(const QByteArray& id, QObject* parent) override;
	QString pluginName() const override { return "TemplateDevice"; }
	Device::Type sourceType() const override { return Device::Type::OTHER; }
};
