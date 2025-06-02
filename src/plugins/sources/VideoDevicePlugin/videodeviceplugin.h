#pragma once

#include <QObject>
#include "interfaces/plugins/isourceplugin.h"
#include "sources/Source/source.h"
#include "VideoDevice/videodevice.h"
#include <QtMultimedia>

class VideoDevicePlugin : public QObject, public ISourcePlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID ISourcePlugin_iid FILE "videodeviceplugin.json")
	Q_INTERFACES(ISourcePlugin)

public:
	QList<SourceInfo> availableSources() const override;
	Source* createSource(const QByteArray& id, QObject* parent) override;
	QString pluginName() const override { return "videodevice"; }
	Source::Type sourceType() const override { return Source::Type::VIDEO; }
};
