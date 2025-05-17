// videoclipbuffer.h
#pragma once
#include "Clipping/Buffers/ClipBufferBase/clipbufferbase.h"
#include <QVideoFrame>
#include <deque>

class VideoClipBuffer : public ClipBufferBase
{
    Q_OBJECT
public:
    using ClipBufferBase::ClipBufferBase; // inherit ctor

    struct Item {
        QVideoFrame frame;
        time timestamp;
    };

    // push a frame with session-relative timestamp (ns)
    void push(const QVideoFrame& f, time tsNs);

    // current snapshot (thread-safe copy)
    std::deque<Item> data() const;

    // overrides
    void clear() override;
    int  size()  const override;

signals:
    void frameAdded(const QVideoFrame&);

private:
    mutable QMutex mLocalMx;
    std::deque<Item> mBuf;
};
