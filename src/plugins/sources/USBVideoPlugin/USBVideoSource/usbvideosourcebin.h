#pragma once

#include <bins/sourcebin.h>
#include <gst/video/videooverlay.h>

class USBVideoSource;

class USBVideoSourceBin : public SourceBin {
public:
	USBVideoSourceBin(const std::string& id, USBVideoSource* source); // TODO/CONSIDER: Is passing the source ptr best practice?

	virtual ~USBVideoSourceBin() = default;

protected:
	virtual bool build() override;
};