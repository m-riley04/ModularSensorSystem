#include "yoloobjectdetectionprocessorplugin.hpp"
#include "YoloObjectDetectionProcessor/yoloobjectdetectionprocessor.hpp"

Processor* YoloObjectDetectionProcessorPlugin::createProcessor(Source* src, QObject* parent)
{
    const ElementInfo info{
        .id = "yolo_object_detection_processor",
        .name = "YOLO Object Detection Processor",
        .displayName = "YOLO Object Detection Processor",
        .pluginId = "yolo_object_detection_processor_plugin",
    };
    return new YoloObjectDetectionProcessor(info, src, parent);
}
