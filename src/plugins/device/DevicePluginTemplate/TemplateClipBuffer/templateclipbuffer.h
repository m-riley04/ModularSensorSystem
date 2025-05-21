#pragma once
#include "clipping/Buffers/ClipBufferBase/clipbufferbase.h"
#include <deque>

class TemplateClipBuffer : public ClipBufferBase
{
    Q_OBJECT
public:
    using ClipBufferBase::ClipBufferBase; // inherit ctor

    void clear() override;
    int  size() const override;
};
