#pragma once

#include <QObject>
#include <QUrl>

#define DEFAULT_RECORD_INTERVAL 1.0

enum SensorState {
	SENSOR_IDLE,
	SENSOR_STARTING,
	SENSOR_STOPPING,
	SENSOR_RECORDING,
	SENSOR_RECORDING_PAUSED
};

class Sensor : public QObject
{
	Q_OBJECT

protected:
	QUrl _outputDir; /// TODO: Maybe change this to QDir
	double _readInterval = DEFAULT_RECORD_INTERVAL;
	SensorState _state = SENSOR_IDLE;

public:
	Sensor(QObject *parent);
	~Sensor();

	QUrl& outputDirectory();
	double readInterval();
	SensorState state();

public slots:

	void setOutputDirectory(QUrl dir);
	void setReadInterval(double interval);
	void setState(SensorState state);

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
	void readIntervalChanged(double interval);
	void stateChanged(SensorState state);
};