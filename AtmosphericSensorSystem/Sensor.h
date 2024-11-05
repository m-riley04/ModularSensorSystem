#pragma once

#include <QObject>
#include <QUrl>

#define DEFAULT_INTERVAL 1.0

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
	QUrl _outputDir;
	double _interval = DEFAULT_INTERVAL;
	SensorState _state = SENSOR_IDLE;

public:
	Sensor(QObject *parent);
	~Sensor();

	/// <summary>
	/// The output directory where the sensor will save the recorded data
	/// </summary>
	/// <returns></returns>
	QUrl& outputDirectory();

	/// <summary>
	/// The interval that the sensor will read data from
	/// </summary>
	/// <returns></returns>
	double interval();

	/// <summary>
	/// The state of the sensor
	/// </summary>
	/// <returns></returns>
	SensorState state();

	/// <summary>
	/// Read the data from the sensor at an instant
	/// </summary>
	/// <returns></returns>
	virtual QVariant read() = 0;

public slots:

	void setOutputDirectory(QUrl dir);
	void setInterval(double interval);
	void setState(SensorState state);


	virtual void open() = 0;
	virtual void release() = 0;
	virtual void restart() = 0;

	virtual void startRecording() = 0;
	virtual void pauseRecording() = 0;
	virtual void stopRecording() = 0;
	
signals:
	void opened();
	void released();
	void outputDirectoryChanged(QUrl dir);
	void recordingStarted();
	void recordingPaused();
	void recordingStopped();
	void recordingError();
	void intervalChanged(double interval);
	void stateChanged(SensorState state);
	void error(QString message);
};