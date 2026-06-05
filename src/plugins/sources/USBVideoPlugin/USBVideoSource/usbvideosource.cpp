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
	return m_recorderBranch->setRecordingEnabled(false); // finalization happens within the branch
}

QList<SettingDescriptor> USBVideoSource::settingsSchema() const
{
	return {
		{ SettingDescriptor::Enum, "resolution", "Resolution", "...", "Video", QVariant(), QVariant(), QVariant(), QStringList()},
		{ SettingDescriptor::Int, "frame_rate", "Frame Rate", "...", "Video", QVariant(), QVariant(), QVariant(), QStringList()},
		{ SettingDescriptor::DirPath, "output_path", "Output Path", "...", "Recording", QVariant(), QVariant(), QVariant(), QStringList()},
	};
}

QVariant USBVideoSource::settingValue(const QString& key) const
{
	return m_settingsMap.value(key);
}

bool USBVideoSource::setSettingValue(const QString& key, const QVariant& value)
{
	m_settingsMap.insert(key, value);
	return true; // TODO: 
}

void USBVideoSource::resetSettings()
{
	// TODO: implement
}

bool USBVideoSource::openOsSettings()
{
	// TOOD: make this work on multiple systems, NOT just Windows w/ dshow
	#ifndef Q_OS_WINDOWS
	LoggingController::warning("Cannot open OS settings, as only Windows is supported at this time.");
	return false;
	#endif

	// check if ffmpeg installed
	if (QStandardPaths::findExecutable("ffmpeg").isEmpty()) {
		LoggingController::warning("Could not find FFMPEG for OS video settings");
		return false;
	}

	QString procName = "ffmpeg";
	QString videoName = QString::fromStdString(this->name());
	QStringList args;
	args << "-f" << "dshow"
		<< "-show_video_device_dialog" << "true"
		<< "-i" << QString("video=%1").arg(videoName);
	QProcess* proc = new QProcess(this);
	connect(proc, &QProcess::finished, proc, &QProcess::deleteLater);
	proc->start(procName, args);
	return true;
}
