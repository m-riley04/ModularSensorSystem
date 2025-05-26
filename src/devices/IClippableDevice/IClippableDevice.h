#pragma once
#include <QWidget>
#include <QDir>

class IClippableDevice {
public:
    virtual ~IClippableDevice() = default;

    virtual void clip(const QDir& outputDir) = 0;
};

#define IClippableDevice_iid "com.rileymeyerkorth.IClippableDevice/1.0"
Q_DECLARE_INTERFACE(IClippableDevice, IClippableDevice_iid)