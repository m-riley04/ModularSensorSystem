#pragma once

#include <QObject>
#include <Controllers/DeviceController/devicecontroller.h>

class MainController  : public QObject
{
	Q_OBJECT

public:
	MainController(QObject *parent);
	~MainController();

	DeviceController* deviceController() const { return pDeviceController.get(); }

private:
	std::unique_ptr<DeviceController> pDeviceController;
};
