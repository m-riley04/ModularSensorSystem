#pragma once

#include <QObject>
#include "devices/Device/device.h"

class IProcessorPlugin {
public:
    virtual ~IProcessorPlugin() = default;
    /// Identify which device modality this processor supports (e.g. CAMERA, MICROPHONE)
    virtual Device::Type deviceType() const = 0;
    /// Human-readable name of the processor (for UI)
    virtual QString pluginName() const = 0;
    /// Create a new processor instance for the given device
    virtual ProcessorBase* createProcessor(Device* device, QObject* parent = nullptr) = 0;
};

#define IProcessorPlugin_iid "com.modularsensorsystem.IProcessorPlugin/1.0"
Q_DECLARE_INTERFACE(IProcessorPlugin, IProcessorPlugin_iid)