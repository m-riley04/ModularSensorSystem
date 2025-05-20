#pragma once

#include <QObject>
#include "Clipping/Buffers/ClipBufferBase/clipbufferbase.h"
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
	enum State {
		IDLE,
		CAPTURING
	};

public:
	ClipController(QObject *parent = nullptr);
	~ClipController();

	void saveClip(const QString& fileName);
	void saveIndividualClip(ClipBufferBase* buffer, const QString& fileName);
	void addClipBuffer(ClipBufferBase* buffer);
	//void removeClipBuffer(ClipBufferBase* buffer) { mClipBuffers.append(buffer); }

private:
	QList<ClipBufferBase*> mClipBuffers;

	AVPixelFormat mPixelFormat = AV_PIX_FMT_YUV420P;
	AVCodecID mEncoderCodec = AV_CODEC_ID_MPEG4;
	int mWidth = 640;
	int mHeight = 480;
	int mFps = 30;
	int mBitrate = 4000000;

	AVFrame* allocateFrame();
	AVFrame* convertQFrameToAVFrame(const QVideoFrame& image, int width, int height);

signals:
	void clipBufferAdded(ClipBufferBase* buffer);
	void clipBufferRemoved(ClipBufferBase* buffer);
	void buffersCleared();
	void clipSaved(const QString& fileName);
};
