#include "usbvideosource.hpp"

USBVideoSource::USBVideoSource(const std::string& hardwareId, QObject* parent)
	: USBVideoSource(getSourceInfo(hardwareId), parent)
{}

USBVideoSource::USBVideoSource(SourceInfo sourceInfo, QObject* parent)
	: Source(sourceInfo.elementInfo, parent),
	m_bin(std::make_unique<USBVideoSourceBin>(this->uuid(), sourceInfo.elementInfo.id))
{}

USBVideoSource::~USBVideoSource()
{
    // TODO: Implement(?)
}

SourceInfo USBVideoSource::getSourceInfo(const std::string& id) const
{
	// Get the camera source from the id
	for (const auto& sourceInfo : getUsbVideoDevices()) {
		if (sourceInfo.elementInfo.id == id) {
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
		m_bin = std::make_unique<USBVideoSourceBin>(this->uuid(), this->id());
	}
}

void USBVideoSource::createRecorderBinIfNeeded()
{
	if (!m_recorderBin) {
		m_recorderBin = std::make_unique<USBVideoSourceRecorderBin>(this->uuid(), this->id());
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
	if (!m_recorderBin) return false;

	return m_recorderBin->setRecordingEnabled(true);
}

bool USBVideoSource::stopRecording()
{
	if (!m_recorderBin) return false;
	// Close the valve FIRST, THEN send the EOS to finalize the file
	return m_recorderBin->setRecordingEnabled(false) && m_recorderBin->finalizeRecording();
}
