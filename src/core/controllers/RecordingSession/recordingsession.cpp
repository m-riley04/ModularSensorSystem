#include "recordingsession.h"

RecordingSession::RecordingSession(const QString& baseDir, QObject *parent)
	: QObject(parent), mDir(baseDir)
{
	// Check if folder exists
	if (!mDir.exists()) {
		if (!mDir.mkpath(mDir.absolutePath())) {
			qDebug() << "Failed to create directory: " << mDir.absolutePath();
		}
	}
}

RecordingSession::~RecordingSession()
{}
