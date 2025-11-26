#pragma once

#include <QObject>
#include <qhash.h>
#include "features/sources/source.hpp"
#include "features/processors/processor.hpp"
#include "controllers/backendcontrollerbase.hpp"
#include "utils/boost_qt_conversions.hpp"

class ProcessingController  : public BackendControllerBase
{
	Q_OBJECT

public:
	ProcessingController(QObject *parent);
	~ProcessingController();

	const QList<Processor*>& processors() { return mProcessors; }

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
	void processorRemoved(QUuid id);
    void processingStarted();
    void processingStopped();
};

