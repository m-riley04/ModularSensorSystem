#pragma once

#include <QObject>

class Device  : public QObject
{
	Q_OBJECT

public:
	Device(QObject *parent);
	~Device();
};
