#pragma once

#include <QObject>
#include <qsize.h>
#include <QtMultimedia>
#include "core/sources/IClippableSource/ClipBufferBase/clipbufferbase.h"

extern "C" {
	#include <libavformat/avformat.h>
	#include <libavcodec/avcodec.h>
	#include <libswscale/swscale.h>
	#include <libavutil/imgutils.h>
	#include <libavutil/opt.h>
}

/// <summary>
/// Helper that encodes raw BGRA/ARGB/RGB32 Qt frames into H.264/MP4.
/// Usage:
///		USBVideoClipEncoder enc(path, QSize(1920, 1080), 30.0);
///		for (auto& it : frames)
///			enc.addFrame(it.frame, it.timestamp);   // timestamp in ns is optional
///		enc.finish();   // or rely on destructor
/// </summary>
class USBVideoClipEncoder
{
	Q_DISABLE_COPY_MOVE(USBVideoClipEncoder)

public:
	USBVideoClipEncoder(QString filePath, QSize frameSize, float frameRate, AVPixelFormat srcPixelFormat = AV_PIX_FMT_BGRA);
	~USBVideoClipEncoder();

	bool isOk() const { return mOk; }

	bool addFrame(const QVideoFrame& frame, qint64 ptsNs);
	void finish();

	QString errorString() const { return mError; }

private:
	bool mOk = false;
	QString mError;

	AVFormatContext* pFormatContext = nullptr;
	AVCodecContext* pCodecContext = nullptr;
	AVStream* pVideoStream = nullptr;
	SwsContext* pSwsContext = nullptr;
	AVFrame* pFrame = nullptr;

	AVPixelFormat mPixelFormat = AV_PIX_FMT_YUV420P;
	int mWidth, mHeight;
	AVRational mTimeBase;
	qint64 mNextPts = 0;

	bool encodeAndWrite(AVFrame* frame);
	AVFrame* allocateFrame();
	void freeAll();

};
