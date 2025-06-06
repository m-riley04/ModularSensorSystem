#include "yoloobjectdetectionprocessorplugin.h"

ProcessorBase* createProcessor(Source* source, QObject* parent)
{
    auto p = new YoloObjectDetectionProcessor(source, parent);

    // Connect device frames to processor
    if (auto vs = qobject_cast<IVideoSource*>(source)) {
        QObject::connect(vs->asQObject(), 
            SIGNAL(frameReady(QVideoFrame)), 
            p, 
            SLOT(processFrame(QVideoFrame)));
    }
    return p;
}
