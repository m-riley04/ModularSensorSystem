#pragma once

#include <QObject>
#include <features/sources/source.h>
#include <features/processors/processor.h>
#include "controllers/backendcontrollerbase.h"
#include <qhash.h>

class ProcessingController  : public BackendControllerBase
{
	Q_OBJECT

public:
	ProcessingController(QObject *parent);
	~ProcessingController();

	QList<Processor*> processors() { return mProcessors; }

    void setActive(bool active) {
		if (active == mActive) return;
		mActive = active;
		active ? emit processingStarted() : emit processingStopped();
    }

	void addProcessor(IProcessorPlugin* plugin);
	void removeProcessor(Processor* processor);

	const Processor* byId(const QUuid& id) const;

private:
	QList<Processor*> mProcessors;
    QHash<QUuid, QList<Processor*>> mSourcesProcessorsMap;
	QHash<QUuid, Processor*> mProcessorsById;
    bool mActive = false;

    bool isCompatible(Processor* proc, Source* src);

signals:
	void processorAdded(Processor* proc);
	void processorRemoved(Processor* proc);
    void processingStarted();
    void processingStopped();
};

