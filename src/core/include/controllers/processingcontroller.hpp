#pragma once

#include <QObject>
#include <qhash.h>
#include <QPointer>
#include "features/sources/source.hpp"
#include "features/processors/processor.hpp"

#include "utils/boost_qt_conversions.hpp"
#include "core_export.hpp"

class MSS_CORE_API ProcessingController  : public QObject
{
	Q_OBJECT

public:
	ProcessingController(QObject *parent);
	~ProcessingController();

	const QList<QPointer<Processor>>& processors() { return mProcessors; }

    void setActive(bool active) {
		if (active == mActive) return;
		mActive = active;
		active ? emit processingStarted() : emit processingStopped();
    }

	void addProcessor(Processor* processor);
	void removeProcessor(Processor* processor);
	void clearProcessors();

	Processor* byId(const QUuid& id) const;

private:
	QList<QPointer<Processor>> mProcessors;
    QHash<QUuid, QList<QPointer<Processor>>> mSourcesProcessorsMap;
	QHash<QUuid, QPointer<Processor>> mProcessorsById;
    bool mActive = false;

    bool isCompatible(Processor* proc, Source* src);

signals:
	void processorAdded(Processor* proc);
	void processorRemoved(QUuid id);
    void processingStarted();
    void processingStopped();
};

