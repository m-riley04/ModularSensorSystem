#pragma once

#include <QObject>
#include "Sensor.h"

class SensorRecordingScheduler : public QObject
{
	Q_OBJECT

private:
	QList<Sensor> _sensors;


public:
	SensorRecordingScheduler(QObject *parent);
	~SensorRecordingScheduler();
};
