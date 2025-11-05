#pragma once

#include <QObject>
#include <QDebug>
#include <bins/sourcebin.h>
#include <gst/video/videooverlay.h>

class USBVideoSourceBin : public SourceBin {
public:
	USBVideoSourceBin(const std::string& id);

	virtual ~USBVideoSourceBin() = default;

protected:
	virtual bool build() override;
};