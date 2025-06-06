#include "recordingcontroller.h"

RecordingController::RecordingController(SourceController* sourceController, QObject *parent)
	: BackendControllerBase("RecordingController", parent), pSourceController(sourceController)
{
	connect(sourceController, &SourceController::sourceAdded, this, &RecordingController::onSourceAdded);
	connect(sourceController, &SourceController::sourceRemoved, this, &RecordingController::onSourceRemoved);
}

RecordingController::~RecordingController()
{}

void RecordingController::start()
{
	// Init session
	QString rootOutputDirName = "output";
	auto dirName = QDateTime::currentDateTime().toString("'Session_'yyyyMMdd_hhmmss");
	pSession.reset(new RecordingSession(QDir::currentPath() + "/" + rootOutputDirName + "/" + dirName, this));

	for (Source* d : pSourceController->sources()) {
		d->beginRecording(pSession.get());
	}

	// Update state
	mState = STARTED;
	emit started(pSession.get()); // sources grab the pointer
}

void RecordingController::stop()
{
	//for (auto d : mSources)  d->stop(); // Probably do not need (starting/stopping is performed by SourceController)
	QDir outputSessionDir = pSession->outputDir();

	// Stop all sources
	for (Source* d : pSourceController->sources()) {
		d->endRecording();
	}
	
	// Reset session
	pSession.reset();

	// Update state
	mState = STOPPED;
	emit stopped(outputSessionDir);
}

void RecordingController::onSourceAdded(Source* source)
{
}

void RecordingController::onSourceRemoved(Source* source)
{
}


