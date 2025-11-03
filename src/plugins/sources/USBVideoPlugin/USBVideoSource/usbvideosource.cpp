#include "usbvideosource.h"

USBVideoSource::USBVideoSource(const std::string& hardwareId, QObject* parent)
	: USBVideoSource(getSourceInfo(hardwareId), parent)
{
    m_id = hardwareId;
}

USBVideoSource::USBVideoSource(SourceInfo sourceInfo, QObject* parent)
	: Source(parent)
{
    m_id = sourceInfo.id;
    m_name = sourceInfo.displayName;
	m_pluginId = "plugin_usb_video";
    mSourceType = Source::Type::VIDEO;
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
    if (mState == CLOSED || mState == STOPPED) return;
	// TODO: Implement close
}

void USBVideoSource::onNewFrame(const QVideoFrame& frame) {
	// Emit signal with the frame
	emit frameReady(frame);
}

void USBVideoSource::setMediaDirectory(QUrl directory)
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

void USBVideoSource::restart() {
    stop();
    start();
}

