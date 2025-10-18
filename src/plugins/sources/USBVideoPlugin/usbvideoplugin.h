#pragma once

#include <QObject>
#include "sdk/plugins/isourceplugin.h"
#include "features/sources/source.h"
#include "USBVideoSource/usbvideosource.h"
#include <QtMultimedia>

class USBVideoPlugin : public QObject, public ISourcePlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID ISourcePlugin_iid FILE "usbvideoplugin.json")
	Q_INTERFACES(ISourcePlugin)

public:
	std::vector<SourceInfo> availableSources() const override;
	Source* createSource(const std::string& id, QObject* parent) override;
	std::string name() const override { return "USB Video"; }
	uint32_t version() const override { return 0x00010000;  }
	Source::Type sourceType() const override { return Source::Type::VIDEO; }
};
