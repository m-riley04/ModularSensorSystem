#pragma once

#include <QObject>
#include <QUrl>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QVariant>
#include "Devices/Device/device.h"

#define DEFAULT_INTERVAL 1.0

class Sensor : public Device
{
	Q_OBJECT

protected:
	double _interval = DEFAULT_INTERVAL;
	QSerialPort _serialPort;
	int _running = false;

public:
	Sensor(QObject *parent);
	~Sensor();

	/// <summary>
	/// The interval that the sensor will read data from
	/// </summary>
	/// <returns></returns>
	double interval();

	/// <summary>
	/// Read the data from the sensor at an instant
	/// </summary>
	/// <returns></returns>
	virtual QVariant read() = 0;

public slots:

	void setInterval(double interval);
	void setPortName(const QString& portName);

	virtual void initialize() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void restart() = 0;
	
signals:
	void started();
	void stopped();

	void dataReady(QVariant data, qint64 timestamp);

	void intervalChanged(double interval);
	void error(QString message);
};