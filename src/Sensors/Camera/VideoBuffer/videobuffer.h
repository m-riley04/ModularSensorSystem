#pragma once

#include <QObject>
#include <deque>
#include <QImage>
#include <QVideoFrame>

class VideoBuffer  : public QObject
{
	Q_OBJECT

public:
	VideoBuffer(double clipDurationMinutes, double frameRate, QObject* parent = nullptr);
	~VideoBuffer();

	/// <summary>
	/// Clears the buffer.
	/// </summary>
	void clear();

	/// <summary>
	/// Returns the buffering state.
	/// </summary>
	/// <returns></returns>
	bool isBuffering() const { return mIsBuffering; }

	/// <summary>
	/// Gets the max frames queued.
	/// </summary>
	/// <returns></returns>
	size_t maxFrames() const { return mMaxFrames; }

	/// <summary>
	/// Gets the current frames queued.
	/// </summary>
	/// <returns></returns>
	size_t frames() const { return mFrames; }

	/// <summary>
	/// Adds a frame to the buffer. If the buffer is full, the oldest frame will be removed.
	/// </summary>
	/// <param name="frame"></param>
	void addFrame(const QVideoFrame& frame);

	/// <summary>
	/// Gets a copy of the current buffered frames.
	/// </summary>
	/// <returns></returns>
	std::deque<QVideoFrame> getFrames() const { return mBuffer; }

public slots:
	void setClipDuration(double clipDurationMinutes);
	void setFrameRate(double frameRate);
	void setIsBuffering(bool isBuffering) { mIsBuffering = isBuffering; }

signals:
	void frameAdded(const QVideoFrame& frame);
	void bufferCleared();
	void clipDurationChanged(double clipDurationMinutes);
	void frameRateChanged(double frameRate);

private:
	size_t mMaxFrames;
	size_t mFrames;
	bool mIsBuffering = false;
	double mFrameRate;
	double mClipDurationMinutes;
	std::deque<QVideoFrame> mBuffer;
};
