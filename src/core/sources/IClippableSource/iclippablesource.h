#pragma once
#include <QWidget>
#include <QDir>
#include "ClipBufferBase/clipbufferbase.h"

class IClippableSource {
public:
    virtual ~IClippableSource() = default;

    /// <summary>
    /// Clips data and writes the result to the specified output directory.
    /// </summary>
    /// <param name="outputDir">The directory where the clipped output will be saved.</param>
    virtual void clip(const QDir& outputDir) = 0;

	virtual ClipBufferBase* clipBuffer() = 0;
};

#define IClippableSource_iid "com.rileymeyerkorth.IClippableSource/1.0"
Q_DECLARE_INTERFACE(IClippableSource, IClippableSource_iid)