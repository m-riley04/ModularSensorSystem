#include "usbvideopreview.h"

USBVideoPreview::USBVideoPreview(QMediaCaptureSession* session, QUuid sourceId, QObject*parent)
	: SourcePreview(sourceId, parent), mSink(QVideoSink(this))
{
	// Connect the video sink to the session
	session->setVideoSink(&mSink);

	// Connect the frame changed signal to the slot
	connect(&mSink, &QVideoSink::videoFrameChanged, this, &USBVideoPreview::onVideoFrameChanged);
}

USBVideoPreview::~USBVideoPreview()
{}

void USBVideoPreview::onVideoFrameChanged(const QVideoFrame& frame)
{
	QImage img = frame.toImage();
	emit frameReady(img);
}