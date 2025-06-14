#pragma once

#include <QObject>
#include <sources/Source/source.h>
#include <processing/ProcessorBase/processorbase.h>
#include "controllers/BackendControllerBase/backendcontrollerbase.h"

class ProcessingController  : public BackendControllerBase
{
	Q_OBJECT

public:
	ProcessingController(QObject *parent);
	~ProcessingController();

	QList<ProcessorBase*> processors() { return mProcessors; }

    void setActive(bool active) {
		if (active == mActive) return;
		mActive = active;
		active ? emit processingStarted() : emit processingStopped();
    }

	void addProcessor(IProcessorPlugin* plugin);
	void removeProcessor(ProcessorBase* processor);

private:
	QList<ProcessorBase*> mProcessors;
    QMap<Source*, QList<ProcessorBase*>> mSourcesProcessorsMap;
    bool mActive = false;

	QHash<QUuid, QList<ProcessorBase*>> mProcessorsBySourceId;

    bool isCompatible(ProcessorBase* proc, Source* src);

signals:
	void processorAdded(ProcessorBase* proc);
	void processorRemoved(ProcessorBase* proc);
    void processingStarted();
    void processingStopped();
};

