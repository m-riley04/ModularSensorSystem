#pragma once

#include <QObject>

class DeviceController  : public QObject
{
	Q_OBJECT

public:
	DeviceController(QObject *parent);
	~DeviceController();
};
