#pragma once

#include <QObject>
#include <qsize.h>
#include <QtMultimedia>
#include "devices/IClippableDevice/ClipBufferBase/clipbufferbase.h"

class VideoClipEncoder
{

public:
	VideoClipEncoder(QString filePath, QSize frameSize, float frameRate);
	~VideoClipEncoder();

	void addFrame(QVideoFrame frame, ClipBufferBase::time timestamp);
	void finish();
};
