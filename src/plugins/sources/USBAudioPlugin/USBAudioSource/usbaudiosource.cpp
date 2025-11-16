#include "usbaudiosource.hpp"

USBAudioSource::USBAudioSource(const std::string& hardwareId, QObject* parent)
	: USBAudioSource(getSourceInfo(hardwareId), parent)
{}

USBAudioSource::USBAudioSource(SourceInfo sourceInfo, QObject* parent)
	: Source(parent), m_id(sourceInfo.id), m_name(sourceInfo.displayName),
	m_bin(std::make_unique<USBAudioSourceBin>(this->uuid(), sourceInfo.id))
{}

USBAudioSource::~USBAudioSource()
{
    // TODO: Implement(?)
}

SourceInfo USBAudioSource::getSourceInfo(const std::string& id) const
{
	// Get the camera source from the id
	for (const auto& sourceInfo : getUsbAudioDevices()) {
		if (sourceInfo.id == id) {
			return sourceInfo;
		}
	}
    
	// If not found, return an empty source info
    return SourceInfo();
}

void USBAudioSource::onSessionStart()
{

}

void USBAudioSource::onSessionStop()
{
	// Reset bin
	m_bin.reset(nullptr);
}

void USBAudioSource::createBinIfNeeded()
{
	if (!m_bin) {
		m_bin = std::make_unique<USBAudioSourceBin>(this->uuid(), m_id);
	}
}

GstElement* USBAudioSource::srcBin()
{
	// lazy creation; note m_bin must be mutable
	createBinIfNeeded();
	return m_bin->bin();
}