#pragma once

#include <QObject>
#include "Devices/CameraDevice/VideoBuffer/videobuffer.h"
#include <QDebug>
#include <QVideoFrame>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
}

class ClipController : public QObject
{
	Q_OBJECT

public:
	ClipController(VideoBuffer* buffer, int width, int height, double fps, int bitrate, QObject *parent = nullptr);
	~ClipController();

	VideoBuffer* videoBuffer() const { return pBuffer; }

public slots:
	void saveClip(const QString& fileName);

private:
	VideoBuffer* pBuffer = nullptr;

	AVPixelFormat mPixelFormat = AV_PIX_FMT_YUV420P;
	AVCodecID mEncoderCodec = AV_CODEC_ID_MPEG4;
	int mWidth = 640;
	int mHeight = 480;
	int mFps = 30;
	int mBitrate = 4000000;

	AVFrame* allocateFrame();
	AVFrame* convertQFrameToAVFrame(const QVideoFrame& image, int width, int height);
};
