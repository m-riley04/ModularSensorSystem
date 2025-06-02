#pragma once

#include <QObject>
#include <sources/Source/source.h>
#include <Controllers/RecordingSession/recordingsession.h>
#include <qcameradevice.h>
#include <controllers/PluginController/plugincontroller.h>
#include <sources/Source/SourceError/sourceerror.h>

class SourceController : public QObject
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

private:
	QList<Source*> mSources;
	SourceController::State mState = CLOSED;

	QPointer<PluginController> pPluginController;
	RecordingSession* pRecordingSession;

public slots:
	Source* addSource(ISourcePlugin* plugin, SourceInfo info);
	void removeSource(Source* device);
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

	void sourceAdded(Source* device);
	void sourceRemoved(Source* device);

	void errorOccurred(const SourceError& e);
};
