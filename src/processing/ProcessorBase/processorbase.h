#pragma once

#include <QObject>
#include "devices/Device/device.h"
#include "interfaces/plugins/iprocessorplugin.h"

class ProcessorBase : public QObject 
{
    Q_OBJECT

public:
    ProcessorBase(Device* device, QObject* parent = nullptr);
    virtual ~ProcessorBase() = default;

    Device* device() const { return pDevice; }

	void setDevice(Device* device) { 
		if (device == pDevice) return;
        pDevice = device;
		emit deviceChanged(pDevice);
    }

    virtual void startProcessing() {}
    virtual void stopProcessing() {}

protected:
    Device* pDevice;

signals:
	void deviceChanged(Device* newDevice);

    // Common signals for detection events (can be extended or specialized per plugin)
    void motionDetected(Device* sourceDevice);
    void personDetected(Device* sourceDevice);
	void objectDetected(Device* sourceDevice, const QString& objectType); // TODO: Use a struct or class for object details
};
