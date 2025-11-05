#include "usbvideosource.h"

USBVideoSource::USBVideoSource(const std::string& hardwareId, QObject* parent)
	: USBVideoSource(getSourceInfo(hardwareId), parent)
{}

USBVideoSource::USBVideoSource(SourceInfo sourceInfo, QObject* parent)
	: Source(parent), m_id(sourceInfo.id), m_name(sourceInfo.displayName), m_sourceBin(std::make_unique<USBVideoSourceBin>(sourceInfo.id))
{}

USBVideoSource::~USBVideoSource()
{
    // TODO: Implement(?)
}

SourceInfo USBVideoSource::getSourceInfo(const std::string& id) const
{
	// Get the camera source from the id
	for (const auto& sourceInfo : getUsbVideoDevices()) {
		if (sourceInfo.id == id) {
			return sourceInfo;
		}
	}
    
	// If not found, return an empty source info
    return SourceInfo();
}
