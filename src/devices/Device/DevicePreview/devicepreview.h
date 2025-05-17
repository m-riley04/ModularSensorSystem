#pragma once

#include <QObject>
#include <devices/VideoDevice/videodevice.h>

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
