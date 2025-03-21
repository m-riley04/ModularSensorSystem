#pragma once

#include <QObject>
#include "Controllers/SensorController/SensorController.h"

class MainController  : public QObject
{
	Q_OBJECT

public:
	MainController(QObject *parent);
	~MainController();

	SensorController* sensorController() const { return pSensorController.get(); }

private:
	std::unique_ptr<SensorController> pSensorController;
};
