#pragma once
#include "features/sources/clipbufferbase.h"
#include <QVideoFrame>
#include <deque>

class USBVideoClipBuffer : public ClipBufferBase
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
    void flush() override;
    int  size()  const override;

signals:
    void frameAdded(const QVideoFrame&);

private:
    mutable QMutex mLocalMx;
    std::deque<Item> mBuf;
};
