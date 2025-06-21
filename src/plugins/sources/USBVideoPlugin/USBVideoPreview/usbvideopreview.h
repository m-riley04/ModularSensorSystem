#pragma once

#include "core/features/sources/Source/SourcePreview/sourcepreview.h"
#include <qmediacapturesession.h>
#include <QtMultimedia>
#include <QObject>

class USBVideoPreview : public SourcePreview
{
	Q_OBJECT

public:
	USBVideoPreview(QMediaCaptureSession* session, QObject *parent);
	~USBVideoPreview();

private:
	QVideoSink mSink;

private slots:
	void onVideoFrameChanged(const QVideoFrame& frame);
};
