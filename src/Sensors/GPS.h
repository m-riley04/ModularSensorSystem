#pragma once

#include <QObject>
#include "Sensor.h"

class GPS : public Sensor
{
	Q_OBJECT

public:
	GPS(QObject *parent);
	~GPS();
};