#pragma once

#include <QObject>
#include "devices/Device/device.h"
#include <qelapsedtimer.h>
#include "devices/IClippableDevice/ClipBufferBase/clipbufferbase.h"
#include <qdir.h>
#include <QtMultimedia/qmediarecorder.h>

class Device;

class RecordingSession : public QObject
{
	Q_OBJECT

public:
	RecordingSession(const QString& baseDir, QObject *parent);
	~RecordingSession();

	qint64 currentTimeNs() const { return mTimer.nsecsElapsed(); }
	QDir outputDir() const { return mDir; }

private:
	QElapsedTimer mTimer;
	QDir mDir;

signals:
	void started();
	void stopped();

};
