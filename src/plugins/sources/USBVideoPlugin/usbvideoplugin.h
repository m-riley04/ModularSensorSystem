#pragma once

#include <QObject>
#include "core/interfaces/plugins/isourceplugin.h"
#include "core/features/sources/Source/source.h"
#include "USBVideoSource/usbvideosource.h"
#include <QtMultimedia>

class USBVideoPlugin : public QObject, public ISourcePlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID ISourcePlugin_iid FILE "usbvideoplugin.json")
	Q_INTERFACES(ISourcePlugin)

public:
	QList<SourceInfo> availableSources() const override;
	Source* createSource(const QByteArray& id, QObject* parent) override;
	QString pluginName() const override { return "usb_video"; }
	Source::Type sourceType() const override { return Source::Type::VIDEO; }
};
