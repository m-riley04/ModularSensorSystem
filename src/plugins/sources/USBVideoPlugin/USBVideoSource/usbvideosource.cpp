#include "usbvideosource.h"

USBVideoSource::USBVideoSource(const std::string& hardwareId, QObject* parent)
	: USBVideoSource(getSourceInfo(hardwareId), parent)
{}

USBVideoSource::USBVideoSource(SourceInfo sourceInfo, QObject* parent)
	: Source(parent)
{
    m_id = sourceInfo.id;
    m_name = sourceInfo.displayName;

	// Init bin
	m_sourceBin = std::make_unique<USBVideoSourceBin>(sourceInfo.id, this);
}

USBVideoSource::~USBVideoSource()
{
    // TODO: Implement(?)
}

void USBVideoSource::open() {
	// TODO: Implement(?)
}

void USBVideoSource::start() {
	// TODO: Implement(?)
}

void USBVideoSource::stop() {
	// TODO: Implement(?)
}

void USBVideoSource::close()
{
    if (m_state == CLOSED || m_state == STOPPED) return;
	// TODO: Implement close
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

void USBVideoSource::restart() {
    stop();
    start();
}

