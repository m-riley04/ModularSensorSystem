#pragma once

#include <QObject>
#include "SensorController.h"

class MainController : public QObject
{
	Q_OBJECT

private:
	SensorController* sensorController;

public:
	MainController(QObject *parent);
	~MainController();

public slots:
	void startSensors();
	void stopSensors();
};
