#pragma once

#include <QObject>
#include "interfaces/plugins/iprocessorplugin.h"
#include "devices/Device/device.h"
#include "YoloObjectDetectionProcessor/yoloobjectdetectionprocessor.h"
#include <interfaces/capability/ivideosource.h>

class YoloObjectDetectionProcessor;

class YoloObjectDetectionProcessorPlugin : public QObject, public IProcessorPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID IProcessorPlugin_iid FILE "yoloobjectdetectionprocessorplugin.json")
    Q_INTERFACES(IProcessorPlugin)

public:
    Device::Type supportedDeviceType() const override { return Device::Type::VIDEO; }
    QString name() const override { return "yoloobjectdetectionprocessor"; }
    ProcessorBase* createProcessor(Device* device, QObject* parent = nullptr) override
    {
        return new YoloObjectDetectionProcessor(device, parent);
    }
};
