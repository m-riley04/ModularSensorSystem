#pragma once

#include <QObject>

enum SensorWriterState {
	STOPPED,
	STARTED,
	PAUSED
};

class SensorDataWriter  : public QObject
{
	Q_OBJECT

private:
	SensorWriterState _state = STOPPED;

public:
	SensorDataWriter(QObject *parent);
	~SensorDataWriter();

	SensorWriterState state() const;

	virtual void record(const QVariant& data) = 0;

signals:
	void started();
	void paused();
	void stopped();
	void error(QString message);
};
