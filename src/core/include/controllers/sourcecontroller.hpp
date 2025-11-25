#pragma once

#include <QObject>
#include <qcameradevice.h>
#include <QPointer>
#include "controllers/backendcontrollerbase.hpp"
#include "controllers/plugincontroller.hpp"
#include "features/sources/source.hpp"
#include "utils/boost_qt_conversions.hpp"

class SourceController : public BackendControllerBase
{
	Q_OBJECT

public:
	SourceController(PluginController* pluginController, QObject *parent);
	~SourceController();

	QList<Source*> sources() const { return mSources; }
	QList<IPreviewableSource*> previewableSources() const;
	QList<IRecordableSource*> recordableSources() const;

	PluginController* pluginController() const { return pPluginController; }

	bool isEmpty() const { return mSources.isEmpty(); }

	Source* byId(const QUuid& id) const;

private:
	QList<Source*> mSources;

	QHash<QUuid, QList<Processor*>> mProcessorsBySourceId;
	QHash<QUuid, Source*> mSourcesById;

	QPointer<PluginController> pPluginController;

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
