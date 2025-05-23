#pragma once
#include <QObject>
#include <QMutex>
#include <chrono>

class ClipBufferBase : public QObject
{
    Q_OBJECT

public:
    using ns = std::chrono::nanoseconds;
    using time = std::int64_t; // monotonic ns since session start

    explicit ClipBufferBase(double minutes = 2.0, QObject* parent = nullptr);

    bool isBuffering() const { return mBuffering; }
    void setBuffering(bool b) { mBuffering = b; }

    double clipDurationMin() const { return mWindow.count() / 60'000'000'000.0; }
    void setClipDuration(double minutes); // thread-safe

    virtual void clear() = 0; // must wipe subclass container
    virtual int size() const = 0; // items currently held

protected:
    /// <summary>
	/// Subclasses will call this to trim their container
    /// </summary>
    /// <typeparam name="Container">A container type that each subclass will create</typeparam>
    template<typename Container>
    void trim(Container& c, time newestTs)
    {
        while (!c.empty() && // remove items older than window
            (newestTs - c.front().timestamp) > mWindow.count())
            c.pop_front();
    }

    ns window() const { return mWindow; }
    QMutex& mutex() { return mMx; }

signals:
    void clipDurationChanged(double minutes);
    void bufferCleared();

private:
    bool mBuffering = true;
    ns mWindow;
    mutable QMutex mMx;
};
