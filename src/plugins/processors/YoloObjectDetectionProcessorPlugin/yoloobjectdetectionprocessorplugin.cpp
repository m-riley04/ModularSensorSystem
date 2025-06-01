#include "yoloobjectdetectionprocessorplugin.h"

ProcessorBase* createProcessor(Device* device, QObject* parent)
{
    auto p = new YoloObjectDetectionProcessor(device, parent);

    // Connect device frames to processor
    if (auto vs = qobject_cast<IVideoSource*>(device)) {
        QObject::connect(vs->asQObject(), 
            SIGNAL(frameReady(QVideoFrame)), 
            p, 
            SLOT(processFrame(QVideoFrame)));
    }
    return p;
}
