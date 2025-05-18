#pragma once

#include <QObject>
#include "Devices/Device/device.h"
#include <qelapsedtimer.h>
#include "Clipping/Buffers/ClipBufferBase/clipbufferbase.h"
#include <qdir.h>
#include <QtMultimedia/qmediarecorder.h>

class Device;

class RecordingSession  : public QObject
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
