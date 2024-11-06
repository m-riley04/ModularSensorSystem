#pragma once

#include <QObject>

enum SensorWriterState {
	STOPPED,
	STARTED,
	PAUSED
};

class SensorWriter  : public QObject
{
	Q_OBJECT

private:
	SensorWriterState _state = STOPPED;

public:
	SensorWriter(QObject *parent);
	~SensorWriter();

	SensorWriterState state() const;

	virtual void record(const QVariant& data) = 0;

signals:
	void started();
	void paused();
	void stopped();
	void error(QString message);
};
