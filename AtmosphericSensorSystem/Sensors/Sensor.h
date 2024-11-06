#pragma once

#include <QObject>
#include <QUrl>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QVariant>

#define DEFAULT_INTERVAL 1.0

enum SensorState {
	SENSOR_IDLE,
	SENSOR_RUNNING,
	SENSOR_STARTING,
	SENSOR_STOPPING
};

class Sensor : public QObject
{
	Q_OBJECT

protected:
	QUrl _outputDir;
	double _interval = DEFAULT_INTERVAL;
	QSerialPort _serialPort;
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
	void setPortName(const QString& portName);

	virtual void initialize() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void restart() = 0;
	
signals:
	void started();
	void stopped();

	void dataReady(QVariant data);

	void outputDirectoryChanged(QUrl dir);
	void intervalChanged(double interval);
	void stateChanged(SensorState state);
	void error(QString message);
};