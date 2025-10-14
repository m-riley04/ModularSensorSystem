#include "usbvideoclipencoder.h"

USBVideoClipEncoder::USBVideoClipEncoder(QString filePath, QSize frameSize, 
    float frameRate) : mOk(false) {}

USBVideoClipEncoder::~USBVideoClipEncoder()
{
    if (mOk) finish();
}

bool USBVideoClipEncoder::addFrame(const QVideoFrame& frame, qint64 ptsNs)
{
    // TODO: Implement
    return false;
}

void USBVideoClipEncoder::finish() {
    if (!mOk) return;
    // TODO: Implement
    mOk = false;
}