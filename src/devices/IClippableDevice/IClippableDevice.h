#pragma once
#include <QWidget>
#include <QDir>

class IClippableDevice {
public:
    virtual ~IClippableDevice() = default;

    /// <summary>
    /// Clips data and writes the result to the specified output directory.
    /// </summary>
    /// <param name="outputDir">The directory where the clipped output will be saved.</param>
    virtual void clip(const QDir& outputDir) = 0;

	virtual ClipBufferBase* clipBuffer() = 0;
};

#define IClippableDevice_iid "com.rileymeyerkorth.IClippableDevice/1.0"
Q_DECLARE_INTERFACE(IClippableDevice, IClippableDevice_iid)