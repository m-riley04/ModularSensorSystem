#include "usbvideosource.hpp"

USBVideoSource::USBVideoSource(const std::string& hardwareId, QObject* parent)
	: USBVideoSource(getSourceInfo(hardwareId), parent)
{}

USBVideoSource::USBVideoSource(SourceInfo sourceInfo, QObject* parent)
	: Source(parent), m_id(sourceInfo.id), m_name(sourceInfo.displayName),
	m_bin(std::make_unique<USBVideoSourceBin>(this->uuid(), sourceInfo.id))
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
		m_bin = std::make_unique<USBVideoSourceBin>(this->uuid(), m_id);
	}
}

void USBVideoSource::createRecorderBinIfNeeded()
{
	if (!m_recorderBin) {
		m_recorderBin = std::make_unique<USBVideoSourceRecorderBin>(this->uuid(), m_id);
	}
}

GstElement* USBVideoSource::srcBin()
{
	// lazy creation; note m_bin must be mutable
	createBinIfNeeded();
	return m_bin->bin();
}

GstElement* USBVideoSource::recorderSinkBin()
{
	createRecorderBinIfNeeded();
	return m_recorderBin->bin();
}

std::string USBVideoSource::recorderFileExtension() const
{
	return "mp4";
}

bool USBVideoSource::setRecordingFilePath(const std::string& filePath)
{
	return m_recorderBin->setRecordingFilePath(filePath);
}

bool USBVideoSource::startRecording()
{
	return m_recorderBin->setRecordingEnabled(true);
}

bool USBVideoSource::stopRecording()
{
	// Close the valve FIRST, THEN send the EOS to finalize the file
	return m_recorderBin->setRecordingEnabled(false) && m_recorderBin->finalizeRecording();
}
