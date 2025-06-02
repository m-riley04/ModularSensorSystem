#include "usbvideoclipencoder.h"

static QString avErr(int err)
{
    char buf[AV_ERROR_MAX_STRING_SIZE]{};
    av_strerror(err, buf, sizeof(buf));
    return QString::fromLocal8Bit(buf);
}

USBVideoClipEncoder::USBVideoClipEncoder(QString filePath, QSize frameSize, float frameRate, AVPixelFormat srcPixelFormat)
	:   mPixelFormat(srcPixelFormat), mWidth(frameSize.width()), mHeight(frameSize.height()), 
        mTimeBase{ 1, int(std::round(frameRate * 1000)) }, mNextPts(0), mOk(false)
{
    int ret;
    
    // Create output format
    if ((ret = avformat_alloc_output_context2(&pFormatContext, nullptr, "mp4", filePath.toUtf8().constData())) < 0) {
        mError = avErr(ret); return;
    }

    const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) { mError = "H264 encoder not found"; return; }

    pVideoStream = avformat_new_stream(pFormatContext, codec);
    if (!pVideoStream) { mError = "Could not create stream"; return; }

    pCodecContext = avcodec_alloc_context3(codec);
    pCodecContext->codec_id = codec->id;
    pCodecContext->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecContext->time_base = AVRational{ 1, int(std::round(frameRate * 1000)) };
    pCodecContext->framerate = AVRational{ int(std::round(frameRate * 1000)), 1000 };
    mTimeBase = pCodecContext->time_base;
    pCodecContext->width = mWidth;
    pCodecContext->height = mHeight;
    pCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;
    pCodecContext->bit_rate = 4'000'000;
    pCodecContext->gop_size = 30;
    av_opt_set(pCodecContext->priv_data, "preset", "veryfast", 0);

    if (pFormatContext->oformat->flags & AVFMT_GLOBALHEADER)
        pCodecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    if ((ret = avcodec_open2(pCodecContext, codec, nullptr)) < 0)
    {
        mError = avErr(ret); return;
    }

    ret = avcodec_parameters_from_context(pVideoStream->codecpar, pCodecContext);
    if (ret < 0) { mError = avErr(ret); return; }

	// Open file for writing
    if (!(pFormatContext->oformat->flags & AVFMT_NOFILE)) {
        if ((ret = avio_open(&pFormatContext->pb, filePath.toUtf8().constData(), AVIO_FLAG_WRITE)) < 0)
        {
            mError = avErr(ret); return;
        }
    }

    if ((ret = avformat_write_header(pFormatContext, nullptr)) < 0)
    {
        mError = avErr(ret); return;
    }

    // Swscale if needed
    if (mPixelFormat != AV_PIX_FMT_YUV420P)
        pSwsContext = sws_getContext(mWidth, mHeight, mPixelFormat,
            mWidth, mHeight, AV_PIX_FMT_YUV420P,
            SWS_BICUBIC, nullptr, nullptr, nullptr);

    pFrame = allocateFrame();
    if (!pFrame) { mError = "cannot allocate frame"; return; }

    mOk = true;

}

USBVideoClipEncoder::~USBVideoClipEncoder()
{
    if (mOk) finish();
    freeAll();
}

bool USBVideoClipEncoder::addFrame(const QVideoFrame& frame, qint64 ptsNs)
{
    if (!mOk) return false;
	QVideoFrame qFrame(frame); // copy (unsure if needed, but safer)
    if (!qFrame.isMapped())
        qFrame.map(QVideoFrame::ReadOnly);

    // source data
    int plane = 0; // TODO/CONSIDER: using 0, but could be multiple planes. ensure this is correct for the pixel format
	const uchar* src = qFrame.bits(plane); 
    int srcStride = qFrame.bytesPerLine(plane);

    // convert / copy into pFrame
    if (pSwsContext) {
        const uint8_t* srcSlice[1] = { src };
        int srcStrideArr[1] = { srcStride };
        sws_scale(pSwsContext, srcSlice, srcStrideArr, 0, mHeight,
            pFrame->data, pFrame->linesize);
    }
    else {
        av_image_fill_arrays(pFrame->data, pFrame->linesize,
            src, mPixelFormat, mWidth, mHeight, 1);
    }

    // pts
    if (ptsNs > 0) {
        // convert ns to stream timebase
        pFrame->pts = ptsNs / (1000000000LL * mTimeBase.num / mTimeBase.den);
    }
    else {
        pFrame->pts = mNextPts++;
    }

    return encodeAndWrite(pFrame);
}

void USBVideoClipEncoder::finish() {
    if (!mOk) return;
    encodeAndWrite(nullptr); // flush encoder
    av_write_trailer(pFormatContext);
    mOk = false;
}

AVFrame* USBVideoClipEncoder::allocateFrame()
{
    AVFrame* f = av_frame_alloc();
    f->format = AV_PIX_FMT_YUV420P;
    f->width = mWidth;
    f->height = mHeight;
    av_frame_get_buffer(f, 32);
    return f;
}

void USBVideoClipEncoder::freeAll()
{
    if (pFrame) av_frame_free(&pFrame);
    if (pSwsContext) sws_freeContext(pSwsContext);
    if (pCodecContext) avcodec_free_context(&pCodecContext);
    if (pFormatContext) {
        if (!(pFormatContext->oformat->flags & AVFMT_NOFILE))
            avio_closep(&pFormatContext->pb);
        avformat_free_context(pFormatContext);
    }
}

bool USBVideoClipEncoder::encodeAndWrite(AVFrame* frm)
{
    int ret = avcodec_send_frame(pCodecContext, frm);
    if (ret < 0) { mError = avErr(ret); return false; }

    AVPacket* pkt = av_packet_alloc(); // TODO: watch this and ensure no memory leakage (there shouldn't be, but upgraded from deprecated av_packet_init())
    while (ret >= 0) {
        ret = avcodec_receive_packet(pCodecContext, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) break;
        if (ret < 0) { mError = avErr(ret); return false; }

        pkt->stream_index = pVideoStream->index;
        pkt->dts = pkt->pts;

        av_packet_rescale_ts(pkt, pCodecContext->time_base, pVideoStream->time_base);
        av_interleaved_write_frame(pFormatContext, pkt);
        av_packet_unref(pkt);
    }
    return true;
}