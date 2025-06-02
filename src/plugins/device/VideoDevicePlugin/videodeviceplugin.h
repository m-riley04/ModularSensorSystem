#pragma once

#include <QObject>
#include "interfaces/plugins/ideviceplugin.h"
#include "devices/Device/device.h"
#include "VideoDevice/videodevice.h"
#include <QtMultimedia>

class VideoDevicePlugin : public QObject, public ISourcePlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID IDevicePlugin_iid FILE "videodeviceplugin.json")
	Q_INTERFACES(ISourcePlugin)

public:
	QList<SourceInfo> availableSources() const override;
	Device* createSource(const QByteArray& id, QObject* parent) override;
	QString pluginName() const override { return "videodevice"; }
	Device::Type sourceType() const override { return Device::Type::VIDEO; }
};
