#include "usbvideosource.h"

USBVideoSource::USBVideoSource(const std::string& hardwareId, QObject* parent)
	: USBVideoSource(getSourceInfo(hardwareId), parent)
{}

USBVideoSource::USBVideoSource(SourceInfo sourceInfo, QObject* parent)
	: Source(parent), m_id(sourceInfo.id), m_name(sourceInfo.displayName), m_bin(std::make_unique<USBVideoSourceBin>(sourceInfo.id))
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

void USBVideoSource::onSessionStart()
{

}

void USBVideoSource::onSessionStop()
{
	// Reset bin
	m_bin.reset(nullptr);
}

void USBVideoSource::createBinIfNeeded()
{
	if (!m_bin) {
		m_bin = std::make_unique<USBVideoSourceBin>(m_id);
	}
}

GstElement* USBVideoSource::gstBin() const
{
	// lazy creation; note m_bin must be mutable
	if (!m_bin) {
		// const_cast is safe here because we're only mutating cache-like state
		auto* self = const_cast<USBVideoSource*>(this);
		self->m_bin = std::make_unique<USBVideoSourceBin>(m_id);
	}
	return m_bin->bin();
}