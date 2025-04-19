#include "videobuffer.h"

VideoBuffer::VideoBuffer(double clipDurationMinutes, double frameRate, QObject* parent)
	: QObject(parent), mFrameRate(frameRate), mClipDurationMinutes(clipDurationMinutes)
{
	mMaxFrames = static_cast<size_t>(mFrameRate * 60 * mClipDurationMinutes);
}

VideoBuffer::~VideoBuffer()
{}

void VideoBuffer::addFrame(const QVideoFrame& frame) {
	if (!mIsBuffering) return;

    mBuffer.push_back(frame);
    if (mBuffer.size() > mMaxFrames) {
        mBuffer.pop_front();
    }
	mFrames++;
	emit frameAdded(frame);
}

void VideoBuffer::setFrameRate(double frameRate)
{
	if (mFrameRate == frameRate) {
		return;
	}
	mFrameRate = frameRate;
	emit frameRateChanged(mFrameRate);

	// Update max frames
	mMaxFrames = static_cast<size_t>(mFrameRate * 60 * mClipDurationMinutes);
}

void VideoBuffer::setClipDuration(double clipDurationMinutes)
{
	if (mClipDurationMinutes == clipDurationMinutes) {
		return;
	}
	mClipDurationMinutes = clipDurationMinutes;
	emit clipDurationChanged(mClipDurationMinutes);

	// Update max frames
	mMaxFrames = static_cast<size_t>(mFrameRate * 60 * mClipDurationMinutes);
}

void VideoBuffer::clear() {
	mBuffer.clear();
	mFrames = 0;
	emit bufferCleared();
}