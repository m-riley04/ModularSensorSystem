#pragma once

#include "devices/Device/DevicePreview/devicepreview.h"
#include "devices/Device/device.h"
#include <qmediacapturesession.h>
#include <qvideoframe.h>
#include <QVideoSink>
#include <QObject>

class VideoPreview : public DevicePreview
{
	Q_OBJECT

public:
	VideoPreview(QMediaCaptureSession* session, QObject *parent);
	~VideoPreview();

private:
	QVideoSink mSink;

private slots:
	void onVideoFrameChanged(const QVideoFrame& frame);
};
