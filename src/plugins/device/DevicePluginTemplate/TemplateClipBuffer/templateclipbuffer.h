#pragma once
#include "devices/IClippableDevice/ClipBufferBase/clipbufferbase.h"
#include <deque>

class TemplateClipBuffer : public ClipBufferBase
{
    Q_OBJECT
public:
    using ClipBufferBase::ClipBufferBase; // inherit ctor

    void flush() override;
    int  size() const override;
};
