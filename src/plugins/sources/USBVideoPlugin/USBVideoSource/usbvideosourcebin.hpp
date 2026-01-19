#pragma once

#include <QObject>
#include <pipeline/bins/sourcebin.hpp>
#include <gst/video/videooverlay.h>

class USBVideoSourceBin : public SourceBin {
public:
	USBVideoSourceBin(Element* element);

	virtual ~USBVideoSourceBin() = default;

protected:
	virtual bool build() override;
};