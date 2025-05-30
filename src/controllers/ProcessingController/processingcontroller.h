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
    QStringList availableProcessorTypes(); 
private:
    QMap<Device*, QList<ProcessorBase*>> mDevicesProcessorsMap;


    bool isCompatible(ProcessorBase* proc, Device* dev);
};

