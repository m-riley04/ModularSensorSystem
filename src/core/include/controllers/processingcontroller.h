#pragma once

#include <QObject>
#include <features/sources/source.h>
#include <features/processors/processorbase.h>
#include "controllers/backendcontrollerbase.h"
#include <qhash.h>

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

	const ProcessorBase* byId(const QUuid& id) const;

private:
	QList<ProcessorBase*> mProcessors;
    QHash<QUuid, QList<ProcessorBase*>> mSourcesProcessorsMap;
	QHash<QUuid, ProcessorBase*> mProcessorsById;
    bool mActive = false;

    bool isCompatible(ProcessorBase* proc, Source* src);

signals:
	void processorAdded(ProcessorBase* proc);
	void processorRemoved(ProcessorBase* proc);
    void processingStarted();
    void processingStopped();
};

