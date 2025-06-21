#pragma once

#include <QObject>
#include <qelapsedtimer.h>
#include <qdir.h>
#include <features/sources/Source/source.h>
#include <QDateTime>
#include "controllers/RecordingSession/recordingsession.h"
#include <controllers/SourceController/sourcecontroller.h>
#include "controllers/BackendControllerBase/backendcontrollerbase.h"

class RecordingController  : public BackendControllerBase
{
	Q_OBJECT

public:
	enum State {
		STARTED,
		STOPPED
	};

public:
	RecordingController(SourceController* sourceController, QObject *parent);
	~RecordingController();

	RecordingController::State state() const { return mState; }
	bool isRecording() const { return mState == STARTED; }
	bool isStopped() const { return mState == STOPPED; }

private:
	QPointer<SourceController> pSourceController;
	std::unique_ptr<RecordingSession> pSession;

	RecordingController::State mState = STOPPED;

public slots:
	void start();
	void stop();

private slots:
	void onSourceAdded(Source* source);
	void onSourceRemoved(Source* source);

signals:
	void started(RecordingSession* session);
	void stopped(QDir output);
};
