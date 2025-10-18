#pragma once

#include <QObject>
#include "sdk/plugins/iprocessorplugin.h"
#include "features/sources/source.h"
#include "YoloObjectDetectionProcessor/yoloobjectdetectionprocessor.h"
#include "interfaces/capability/ivideosource.h"

class YoloObjectDetectionProcessor;

class YoloObjectDetectionProcessorPlugin : public IProcessorPlugin {

public:
    Source::Type supportedSourceType() const override { return Source::Type::VIDEO; }
    std::string name() const override { return "YOLO Object Detection"; }
    uint32_t version() const override { return 0x00010000; }
    ProcessorBase* createProcessor(Source* src, QObject* parent = nullptr) override
    {
        return new YoloObjectDetectionProcessor(src, parent);
    }
};
