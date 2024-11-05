#pragma once

#include <QObject>
#include <QUrl>

enum SensorState {
	SENSOR_IDLE,
	SENSOR_STARTING,
	SENSOR_STOPPING,
	SENSOR_RECORDING_STARTED,
	SENSOR_RECORDING_PAUSED,
	SENSOR_RECORDING_STOPPED,
};

class Sensor : public QObject
{
	Q_OBJECT

protected:
	QUrl outputDir; /// TODO: Maybe change this to QDir

public:
	Sensor(QObject *parent);
	~Sensor();

public slots:

	QUrl& getOutputDirectory();
	void setOutputDirectory(QUrl dir);

	virtual void start() = 0;
	virtual void pause() = 0;
	virtual void stop() = 0;
	virtual void restart() = 0;

	virtual void startRecording() = 0;
	virtual void pauseRecording() = 0;
	virtual void stopRecording() = 0;
	
signals:
	void started();
	void stopped();
	void outputDirectoryChanged(QUrl dir);
	void recordingStarted();
	void recordingPaused();
	void recordingStopped();
	void recordingError();
};