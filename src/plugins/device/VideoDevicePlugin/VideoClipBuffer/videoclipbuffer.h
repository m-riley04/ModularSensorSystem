#pragma once
#include "clipping/Buffers/ClipBufferBase/clipbufferbase.h"
#include <QVideoFrame>
#include <deque>

class VideoClipBuffer : public ClipBufferBase
{
    Q_OBJECT
public:
    using ClipBufferBase::ClipBufferBase;

    struct Item {
        QVideoFrame frame;
        time timestamp;
    };

    void push(const QVideoFrame& f, time tsNs);
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
