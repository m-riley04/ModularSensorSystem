#include "videoclipbuffer.h"

void VideoClipBuffer::push(const QVideoFrame& f, time ts)
{
    if (!isBuffering()) return;

    QMutexLocker L(&mLocalMx);
    mBuf.push_back({ f, ts });
    trim(mBuf, ts); // call helper from base
    L.unlock();

    emit frameAdded(f);
}

std::deque<VideoClipBuffer::Item> VideoClipBuffer::data() const
{
    QMutexLocker L(&mLocalMx);
    return mBuf; // shallow copies QVideoFrame handles
}

void VideoClipBuffer::flush()
{
    QMutexLocker L(&mLocalMx);
    mBuf.clear();
    L.unlock();
    emit flushed();
}

int VideoClipBuffer::size() const
{
    QMutexLocker L(&mLocalMx);
    return static_cast<int>(mBuf.size());
}
