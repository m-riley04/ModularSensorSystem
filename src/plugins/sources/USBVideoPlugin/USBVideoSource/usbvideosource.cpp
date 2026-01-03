#include "usbvideosource.hpp"

USBVideoSource::USBVideoSource(const std::string& hardwareId, QObject* parent)
	: USBVideoSource(getSourceInfo(hardwareId), parent)
{}

USBVideoSource::USBVideoSource(SourceInfo sourceInfo, QObject* parent)
	: Source(sourceInfo.elementInfo, parent),
	m_sourceBin(std::make_unique<USBVideoSourceBin>(this))
{}

USBVideoSource::~USBVideoSource()
{
	LoggingController::info("Tearing down USBVideoSource: " + QString::fromStdString(this->displayName()));
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
	// Reset bins
	m_sourceBin.reset(nullptr);
	m_recorderBranch.reset(nullptr);
	m_previewBranch.reset(nullptr);
}

void USBVideoSource::createBinIfNeeded()
{
	if (!m_sourceBin) {
		m_sourceBin = std::make_unique<USBVideoSourceBin>(this);
	}
}

void USBVideoSource::createRecorderBranchIfNeeded()
{
	if (!m_recorderBranch) {
		m_recorderBranch = std::make_unique<USBVideoSourceRecorderBranch>(this);
	}
}

void USBVideoSource::createPreviewBranchIfNeeded()
{
	if (!m_previewBranch) {
		m_previewBranch = std::make_unique<USBVideoSourcePreviewBranch>(this);
	}
}

GstElement* USBVideoSource::gstSrcBin()
{
	// lazy creation; note m_sourceBin must be mutable
	createBinIfNeeded();
	return m_sourceBin->bin();
}

PreviewBranch* USBVideoSource::previewBranch()
{
	createPreviewBranchIfNeeded();
	return m_previewBranch.get();
}

GstElement* USBVideoSource::previewSinkBin()
{
	createPreviewBranchIfNeeded();
	return m_previewBranch->bin();
}

GstElement* USBVideoSource::recorderSinkBin()
{
	createRecorderBranchIfNeeded();
	return m_recorderBranch->bin();
}

std::string USBVideoSource::recorderFileExtension() const
{
	return "mp4";
}

bool USBVideoSource::setRecordingFilePath(const std::string& filePath)
{
	if (!m_recorderBranch) return false;

	return m_recorderBranch->setRecordingFilePath(filePath);
}

bool USBVideoSource::startRecording()
{
	if (!m_recorderBranch) return false;

	return m_recorderBranch->setRecordingEnabled(true);
}

bool USBVideoSource::stopRecording()
{
	if (!m_recorderBranch) return false;
	// Close the valve FIRST, THEN send the EOS to finalize the file
	return m_recorderBranch->setRecordingEnabled(false) && m_recorderBranch->finalizeRecording();
}
