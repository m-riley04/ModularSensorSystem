#pragma once

#include <QObject>
#include <features/sources/source.h>
#include <controllers/recordingsession.h>
#include <qcameradevice.h>
#include <controllers/plugincontroller.h>
#include <features/sources/sourceerror.h>
#include "controllers/backendcontrollerbase.h"

class SourceController : public BackendControllerBase
{
	Q_OBJECT

public:
	enum State {
		OPENED,
		CLOSED,
		STARTED,
		STOPPED,
		ERROR
	};

public:
	SourceController(PluginController* pluginController, QObject *parent);
	~SourceController();

	SourceController::State state() const { return mState; }
	QList<Source*> sources() const { return mSources; }

	PluginController* pluginController() const { return pPluginController; }

	bool isEmpty() const { return mSources.isEmpty(); }
	bool isOpen() const { return mState == OPENED; }
	bool isStarted() const { return mState == STARTED; }
	bool isStopped() const { return mState == STOPPED; }
	bool isClosed() const { return mState == CLOSED; }
	bool isError() const { return mState == ERROR; }

	Source* byId(const QUuid& id) const;

private:
	QList<Source*> mSources;
	SourceController::State mState = CLOSED;

	QHash<QUuid, QList<Processor*>> mProcessorsBySourceId;
	QHash<QUuid, Source*> mSourcesById;

	QPointer<PluginController> pPluginController;
	RecordingSession* pRecordingSession;

public slots:
	Source* addSource(ISourcePlugin* plugin, SourceInfo info);
	void removeSource(Source*);
	Source* getSource(QByteArray id) const;

	void clearSources();

	void openSources();
	void startSources();
	void stopSources();
	void closeSources();

	void restartSources();

signals:
	void sourcesOpened();
	void sourcesStarted();
	void sourcesStopped();
	void sourcesClosed();

	void sourcesRestarted();

	void stateChanged(SourceController::State state);

	void sourceAdded(Source*);
	void sourceRemoved(Source*);

	void errorOccurred(const SourceError& e);
};
