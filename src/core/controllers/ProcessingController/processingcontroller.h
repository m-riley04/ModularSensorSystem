#pragma once

#include <QObject>
#include <features/sources/Source/source.h>
#include <features/processors/ProcessorBase/processorbase.h>
#include "controllers/BackendControllerBase/backendcontrollerbase.h"
#include <qhash.h>
#include <QtQml>

class ProcessingController  : public BackendControllerBase
{
	Q_OBJECT
		QML_ELEMENT

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

