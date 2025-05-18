#include "recordingcontroller.h"

RecordingController::RecordingController(QObject *parent)
	: QObject(parent)
{}

RecordingController::~RecordingController()
{}

void RecordingController::start()
{
	QString rootOutputDirName = "output";
	auto dirName = QDateTime::currentDateTime()
		.toString("'Session_'yyyyMMdd_hhmmss");
	pSession = std::make_unique<RecordingSession>(QDir::currentPath() + "/" + rootOutputDirName + "/" + dirName);

	emit started(pSession.get());      // devices grab the pointer
}

void RecordingController::stop()
{
	//for (auto d : mDevices)  d->stop(); // Probably do not need (starting/stopping is performed by DeviceController)
	QDir outputSessionDir = pSession->outputDir();

	pSession.reset();
	emit stopped(outputSessionDir);
}


