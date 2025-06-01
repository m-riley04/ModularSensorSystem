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

    void setActive(bool active) {
		if (active == mActive) return;
		mActive = active;
		active ? emit processingStarted() : emit processingStopped();
    }

	void addProcessor(IProcessorPlugin* plugin);
	void removeProcessor(ProcessorBase* processor);

private:
	QList<ProcessorBase*> mProcessors;
    QMap<Device*, QList<ProcessorBase*>> mDevicesProcessorsMap;
    bool mActive = false;

    bool isCompatible(ProcessorBase* proc, Device* dev);

signals:
	void processorAdded(ProcessorBase* proc);
	void processorRemoved(ProcessorBase* proc);
    void processingStarted();
    void processingStopped();
};

