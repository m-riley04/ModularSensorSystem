#pragma once

#include "devices/Device/device.h"

class IProcessorPlugin
{

public:
	virtual ~IProcessorPlugin() = default;

	virtual QString name() const = 0;                      // Name or type of the processor
	virtual Device::Type compatibleDeviceType() const = 0; // e.g. "Video", "Audio" etc., for compatibility checking
	virtual void attachDevice(Device* device) = 0;         // Attach to a device's data stream
	virtual void startProcessing() = 0;
	virtual void stopProcessing() = 0;
};

#define IProcessorPlugin_iid "com.rileymeyerkorth.ModularSensorSystem.IProcessorPlugin/1.0"
Q_DECLARE_INTERFACE(IProcessorPlugin, IProcessorPlugin_iid)