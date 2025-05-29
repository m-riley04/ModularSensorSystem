#pragma once

#include <QObject>
#include <devices/Device/device.h>
#include <processing/ProcessorBase/processorbase.h>

class ProcessingController  : public QObject
{
	Q_OBJECT

public:
	ProcessingController(QObject *parent);
	~ProcessingController();

    bool registerProcessor(Device* device, ProcessorBase* proc);

    void unregisterAll(Device* device);

    // Possibly functions to load plugins, list available processor types, etc.
    // e.g., ["YOLO Detector", "MotionDetector"] etc.
    QStringList availableProcessorTypes(); 
private:
    QMap<Device*, QList<ProcessorBase*>> mProcessors;

    bool isCompatible(ProcessorBase* proc, Device* dev);

    void connectSignalsForProcessor(ProcessorBase* proc);
};

