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

    double clipDurationMin() const { return mWindow.count() / 6.0E10; }
    void setClipDuration(double minutes);

    virtual void flush() = 0;
    virtual int size() const = 0;

public slots:
    void setBuffering(bool b) { mBuffering = b; }

protected:
    /// <summary>
	/// Subclasses will call this to trim their container.
    /// </summary>
    /// <typeparam name="Container">
    ///     A container type that each subclass will create. 
    ///     Must have a `empty()` method, a `front()` method, and a `pop_front()` method.
    ///     Structs contained within the container must have a `timestamp` property.
    /// </typeparam>
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
    void flushed();

private:
    bool mBuffering = true;
    ns mWindow;
    mutable QMutex mMx;
};
