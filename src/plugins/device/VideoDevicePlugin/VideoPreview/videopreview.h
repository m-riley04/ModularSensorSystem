#pragma once

//#include "../../../devices/Device/DevicePreview/devicepreview.h"
#include <qmediacapturesession.h>
#include <QtMultimedia>
#include <QObject>

class DevicePreview;

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
