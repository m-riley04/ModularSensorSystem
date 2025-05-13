#include "clipcontroller.h"

ClipController::ClipController(VideoClipBuffer* buffer, int width, int height, double fps, int bitrate, QObject *parent)
	: QObject(parent), pBuffer(buffer), mWidth(width), mHeight(height), mFps(fps), mBitrate(bitrate)
{

}

ClipController::~ClipController()
{}

AVFrame* ClipController::allocateFrame()
{
    AVFrame* frame = av_frame_alloc();
    if (!frame) {
        qDebug() << "Could not allocate video frame";
        return nullptr;
    }
    frame->format = mPixelFormat;
    frame->width = mWidth;
    frame->height = mHeight;
    int ret = av_image_alloc(frame->data, frame->linesize, mWidth, mHeight, mPixelFormat, 32);
    if (ret < 0) {
        qDebug() << "Could not allocate picture buffer";
        av_frame_free(&frame);
        return nullptr;
    }
    return frame;
}

AVFrame* ClipController::convertQFrameToAVFrame(const QVideoFrame& videoFrame, int width, int height)
{
    // Copy the QVideoFrame; if it is not already mapped, map it for reading.
    QVideoFrame frame = videoFrame;
    if (!frame.isMapped()) {
        if (!frame.map(QVideoFrame::ReadOnly)) {
            qDebug() << "Failed to map QVideoFrame for reading";
            return nullptr;
        }
    }
    // Obtain the QImage representation.
    QImage img = frame.toImage();
    frame.unmap();

    // Convert and scale the QImage.
    QImage convertedImg = img.convertToFormat(QImage::Format_RGB32).scaled(width, height, Qt::KeepAspectRatio);

    // Create a temporary source AVFrame to hold the QImage data in BGRA format.
    AVFrame* srcFrame = av_frame_alloc();
    if (!srcFrame) {
        qDebug() << "Could not allocate source frame";
        return nullptr;
    }
    srcFrame->format = AV_PIX_FMT_BGRA; // QImage::Format_RGB32 is stored as BGRA in memory.
    srcFrame->width = convertedImg.width();
    srcFrame->height = convertedImg.height();

    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_BGRA, srcFrame->width, srcFrame->height, 1);
    uint8_t* buffer = (uint8_t*)av_malloc(numBytes);
    if (!buffer) {
        qDebug() << "Failed to allocate buffer for source frame";
        av_frame_free(&srcFrame);
        return nullptr;
    }
    av_image_fill_arrays(srcFrame->data, srcFrame->linesize, buffer, AV_PIX_FMT_BGRA, srcFrame->width, srcFrame->height, 1);

    // Copy the QImage data into the source frame buffer.
    memcpy(srcFrame->data[0], convertedImg.bits(), numBytes);

    // Allocate the destination frame.
    AVFrame* dstFrame = allocateFrame();
    if (!dstFrame) {
        av_free(buffer);
        av_frame_free(&srcFrame);
        return nullptr;
    }

    // Create a SwsContext for conversion from BGRA to YUV420P.
    struct SwsContext* swsCtx = sws_getContext(srcFrame->width, srcFrame->height, AV_PIX_FMT_BGRA,
        dstFrame->width, dstFrame->height, AV_PIX_FMT_YUV420P,
        SWS_BICUBIC, nullptr, nullptr, nullptr);
    if (!swsCtx) {
        qDebug() << "Could not create SwsContext";
        av_free(buffer);
        av_freep(&dstFrame->data[0]);
        av_frame_free(&dstFrame);
        av_frame_free(&srcFrame);
        return nullptr;
    }

    // Perform the conversion.
    sws_scale(swsCtx, srcFrame->data, srcFrame->linesize, 0, srcFrame->height, dstFrame->data, dstFrame->linesize);

    sws_freeContext(swsCtx);
    av_free(buffer);
    av_frame_free(&srcFrame);
    return dstFrame;
}

void ClipController::saveClip(const QString& fileName)
{
    // Retrieve buffered QVideoFrame objects.
    auto frames = pBuffer->data();
    if (frames.empty()) {
        qDebug() << "No frames in buffer to save.";
        return;
    }

    // Use the first frame's startTime as the base.
    qint64 baseTime = frames.front().timestamp;
    if (baseTime < 0) {
        // Fallback: if the QVideoFrame doesn't have valid timing info.
        baseTime = 0;
    }

    // Allocate output format context for the target file.
    AVFormatContext* fmtCtx = nullptr;
    if (avformat_alloc_output_context2(&fmtCtx, nullptr, nullptr, fileName.toStdString().c_str()) < 0 || !fmtCtx) {
        qDebug() << "Could not create output context.";
        return;
    }

    // For this example, we are using MPEG-4; change to H.264 (AV_CODEC_ID_H264) if available.
    const AVCodec* codec = avcodec_find_encoder(mEncoderCodec);
    if (!codec) {
        qDebug() << "Codec not found.";
        avformat_free_context(fmtCtx);
        return;
    }

    // Create a video stream.
    AVStream* videoStream = avformat_new_stream(fmtCtx, nullptr);
    if (!videoStream) {
        qDebug() << "Failed to create video stream.";
        avformat_free_context(fmtCtx);
        return;
    }
    videoStream->time_base = AVRational{ 1, mFps };
    videoStream->avg_frame_rate = AVRational{ mFps, 1 };

    // Allocate codec context.
    AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
    if (!codecCtx) {
        qDebug() << "Could not allocate codec context.";
        avformat_free_context(fmtCtx);
        return;
    }
    codecCtx->codec_id = codec->id;
    codecCtx->bit_rate = mBitrate;
    codecCtx->width = mWidth;
    codecCtx->height = mHeight;
    codecCtx->time_base = AVRational{ 1, mFps };
    codecCtx->framerate = AVRational{ mFps, 1 };
    codecCtx->gop_size = mFps * 2;  // Keyframe every 2 seconds.
    codecCtx->max_b_frames = 0;
    codecCtx->pix_fmt = mPixelFormat;

    // Set encoder preset if supported.
    if (codecCtx->priv_data)
        av_opt_set(codecCtx->priv_data, "preset", "ultrafast", 0);

    // Open the codec.
    int ret = avcodec_open2(codecCtx, codec, nullptr);
    if (ret < 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(ret, errbuf, sizeof(errbuf));
        qDebug() << "Could not open codec:" << errbuf;
        avcodec_free_context(&codecCtx);
        avformat_free_context(fmtCtx);
        return;
    }

    // Copy codec parameters to the stream.
    if (avcodec_parameters_from_context(videoStream->codecpar, codecCtx) < 0) {
        qDebug() << "Failed to copy codec parameters.";
        avcodec_free_context(&codecCtx);
        avformat_free_context(fmtCtx);
        return;
    }

    // Open the output file.
    if (!(fmtCtx->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&fmtCtx->pb, fileName.toStdString().c_str(), AVIO_FLAG_WRITE) < 0) {
            qDebug() << "Could not open output file.";
            avcodec_free_context(&codecCtx);
            avformat_free_context(fmtCtx);
            return;
        }
    }

    // Write the file header.
    if (avformat_write_header(fmtCtx, nullptr) < 0) {
        qDebug() << "Error writing header.";
        avcodec_free_context(&codecCtx);
        if (!(fmtCtx->oformat->flags & AVFMT_NOFILE))
            avio_close(fmtCtx->pb);
        avformat_free_context(fmtCtx);
        return;
    }

    // --- Encoding loop ---
    int64_t pts = 0;
    for (const VideoClipBuffer::Item& item : frames) {
		QVideoFrame qvframe = item.frame;
		qint64 frameStart = item.timestamp;
        
        // Compute relative time:
        if (frameStart < 0)
            frameStart = 0; // fallback if invalid

        // Calculate pts based on the difference from the base time.
        double deltaSec = (frameStart - baseTime) / 1e6; // converting microseconds to seconds
        int64_t framePts = static_cast<int64_t>(deltaSec * mFps);

        // Convert QVideoFrame to an AVFrame for encoding.
        AVFrame* frame = convertQFrameToAVFrame(qvframe, mWidth, mHeight);
        if (!frame)
            continue;
        frame->pts = framePts;

        // Send the frame to the encoder.
        if (avcodec_send_frame(codecCtx, frame) < 0) {
            qDebug() << "Error sending frame for encoding.";
            av_freep(&frame->data[0]);
            av_frame_free(&frame);
            continue;
        }

        // Retrieve and write all available encoded packets.
        AVPacket* pkt = av_packet_alloc();
        while (avcodec_receive_packet(codecCtx, pkt) == 0) {
            // Rescale the packet timestamp.
            av_packet_rescale_ts(pkt, codecCtx->time_base, videoStream->time_base);
            pkt->stream_index = videoStream->index;
            if (av_interleaved_write_frame(fmtCtx, pkt) < 0) {
                qDebug() << "Error writing packet.";
            }
            av_packet_unref(pkt);
        }
        av_packet_free(&pkt);
        av_freep(&frame->data[0]);
        av_frame_free(&frame);
    }

    // Flush the encoder.
    avcodec_send_frame(codecCtx, nullptr);
    AVPacket* pkt = av_packet_alloc();
    while (avcodec_receive_packet(codecCtx, pkt) == 0) {
        av_packet_rescale_ts(pkt, codecCtx->time_base, videoStream->time_base);
        pkt->stream_index = videoStream->index;
        av_interleaved_write_frame(fmtCtx, pkt);
        av_packet_unref(pkt);
    }
    av_packet_free(&pkt);

    // Write trailer and clean up.
    av_write_trailer(fmtCtx);
    if (!(fmtCtx->oformat->flags & AVFMT_NOFILE))
        avio_close(fmtCtx->pb);
    avcodec_free_context(&codecCtx);
    avformat_free_context(fmtCtx);

    qDebug() << "Clip saved successfully to:" << fileName;
}