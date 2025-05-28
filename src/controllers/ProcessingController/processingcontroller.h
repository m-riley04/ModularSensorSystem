#pragma once

#include <QObject>
#include <devices/Device/device.h>
#include <interfaces/irealtimeprocessor.h>

class ProcessingController  : public QObject
{
	Q_OBJECT

public:
	ProcessingController(QObject *parent);
	~ProcessingController();

    bool registerProcessor(Device* device, IRealTimeProcessor* proc);

    void unregisterAll(Device* device);

    // Possibly functions to load plugins, list available processor types, etc.
    // e.g., ["YOLO Detector", "MotionDetector"] etc.
    QStringList availableProcessorTypes(); 
private:
    QMap<Device*, QList<IRealTimeProcessor*>> processors;

    bool isCompatible(IRealTimeProcessor* proc, Device* dev) {
        return proc->compatibleDeviceType() == dev->deviceType();
    }

    void connectSignalsForProcessor(IRealTimeProcessor* proc);
};

