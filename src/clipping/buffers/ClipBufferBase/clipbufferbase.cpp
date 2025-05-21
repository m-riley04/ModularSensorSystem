#include "clipbufferbase.h"

ClipBufferBase::ClipBufferBase(double minutes, QObject* p)
    : QObject(p),
    mWindow(std::chrono::duration_cast<ns>(std::chrono::duration<double,std::ratio<60>>(minutes)))
{}

void ClipBufferBase::setClipDuration(double minutes)
{
    auto newWin = std::chrono::duration_cast<ns>(std::chrono::duration<double, std::ratio<60>>(minutes));
    if (newWin == mWindow) return;

    {
        QMutexLocker L(&mMx);
        mWindow = newWin; // subclasses will trim on next push
    }

    emit clipDurationChanged(minutes);
}
