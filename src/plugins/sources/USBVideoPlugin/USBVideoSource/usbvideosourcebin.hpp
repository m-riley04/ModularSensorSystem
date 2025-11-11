#pragma once

#include <QObject>
#include <QDebug>
#include <bins/sourcebin.hpp>
#include <gst/video/videooverlay.h>

class USBVideoSourceBin : public SourceBin {
public:
	USBVideoSourceBin(const boost::uuids::uuid& uuid, const std::string& id);

	virtual ~USBVideoSourceBin() = default;

protected:
	virtual bool build() override;
};