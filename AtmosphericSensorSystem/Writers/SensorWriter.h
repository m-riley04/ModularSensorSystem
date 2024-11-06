#pragma once

#include <QObject>

enum SensorWriterState {
	STOPPED,
	STARTED,
	PAUSED
};

class SensorWriter : public QObject
{
	Q_OBJECT

private:
	SensorWriterState _state = STOPPED;

public:
	SensorWriter();
	~SensorWriter();

	SensorWriterState state() const;

public slots:
	virtual void record(const QVariant& data) = 0;
};
