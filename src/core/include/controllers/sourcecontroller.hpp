#pragma once

#include <QObject>
#include <qcameradevice.h>
#include <QPointer>
#include <QDebug>
#include <QUuid>

#include "features/processors/processor.hpp"
#include <sdk/plugins/isourceplugin.hpp>
#include "features/sources/source.hpp"
#include "utils/boost_qt_conversions.hpp"

class SourceController : public QObject
{
	Q_OBJECT

public:
	SourceController(QObject *parent);
	~SourceController();

	const QList<Source*>& sources() const { return mSources; }
	const QList<IPreviewableSource*> previewableSources() const;
	const QList<IRecordableSource*> recordableSources() const;

	bool isEmpty() const { return mSources.isEmpty(); }

	Source* byId(const QUuid& id) const;

private:
	QList<Source*> mSources;

	QHash<QUuid, QList<Processor*>> mProcessorsBySourceId;
	QHash<QUuid, Source*> mSourcesById;

public slots:
	Source* addSource(ISourcePlugin* plugin, SourceInfo info);
	void removeSource(Source* src);
	void removeSource(const QUuid& uuid);
	Source* getSource(QByteArray id) const;

	void clearSources();

signals:
	void sourceAdded(Source*);
	void sourceRemoved(QUuid);

};
