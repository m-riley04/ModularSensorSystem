#pragma once

#include <QObject>
#include "devices/Device/device.h"
#include "processing/ProcessorBase/processorbase.h"

class ProcessorBase;

struct ProcessorInfo {
    QString name;
    Device::Type supportedDeviceType;
};

class IProcessorPlugin {
public:
    virtual ~IProcessorPlugin() = default;
    virtual Device::Type supportedDeviceType() const = 0;
    virtual QString name() const = 0;
    virtual ProcessorBase* createProcessor(Device* device, QObject* parent = nullptr) = 0;
};

#define IProcessorPlugin_iid "com.modularsensorsystem.IProcessorPlugin/1.0"
Q_DECLARE_INTERFACE(IProcessorPlugin, IProcessorPlugin_iid)