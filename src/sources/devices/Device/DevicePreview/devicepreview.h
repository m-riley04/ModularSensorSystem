#pragma once

#include <QObject>
#include "devices/Device/device.h"

class Device;

class DevicePreview  : public QObject
{
	Q_OBJECT

public:
	DevicePreview(QObject *parent);
	~DevicePreview();

	Device* device() const { return pDevice; }

private:
	Device* pDevice = nullptr;

signals:
	void frameReady(const QImage& frame);
};
