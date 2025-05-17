#include "videopreview.h"

VideoPreview::VideoPreview(QMediaCaptureSession* session, QObject*parent)
	: DevicePreview(parent), mSink(QVideoSink(this))
{
	// Connect the video sink to the session
	session->setVideoSink(&mSink);

	// Connect the frame changed signal to the slot
	connect(&mSink, &QVideoSink::videoFrameChanged, this, &VideoPreview::onVideoFrameChanged);
}

VideoPreview::~VideoPreview()
{}

void VideoPreview::onVideoFrameChanged(const QVideoFrame& frame)
{
	QImage img = frame.toImage();
	qDebug() << "emitter" << this  // check address against widget debug
		<< "img isNull?" << img.isNull();
	emit frameReady(img);
}