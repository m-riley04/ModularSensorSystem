#pragma once

#include <QObject>
#include <QUrl>
#include "SensorController.h"

class MainController : public QObject
{
	Q_OBJECT

private:
	SensorController* sensorController;

public:
	MainController(QObject *parent, QLabel* label);
	~MainController();

public slots:
	void startSensors();
	void stopSensors();
};
