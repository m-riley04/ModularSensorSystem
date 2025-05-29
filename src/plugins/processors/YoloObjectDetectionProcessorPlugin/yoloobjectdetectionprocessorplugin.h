#pragma once

#include <QObject>
#include "interfaces/plugins/iprocessorplugin.h"
#include "devices/Device/device.h"
#include "YoloObjectDetectionProcessor/yoloobjectdetectionprocessor.h"
#include <interfaces/capability/ivideosource.h>

class YoloObjectDetectionProcessor;  // the actual processor logic class for video

class YoloObjectDetectionProcessorPlugin : public QObject, public IProcessorPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID IProcessorPlugin_iid FILE "yoloobjectdetectionprocessorplugin.json")
    Q_INTERFACES(IProcessorPlugin)

public:
    Device::Type deviceType() const override { return Device::Type::VIDEO; }
    QString name() const override { return "yoloobjectdetectionprocessor"; }
    ProcessorBase* createProcessor(Device* device, QObject* parent = nullptr) override {
        // create and return a new VideoProcessor instance attached to the given device
        return new YoloObjectDetectionProcessor(device, parent);
    }
};
