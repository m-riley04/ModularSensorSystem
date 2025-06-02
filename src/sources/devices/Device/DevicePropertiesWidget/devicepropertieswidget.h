#pragma once

#include <QWidget>
#include <QtWidgets>
#include "devices/Device/device.h"

class Device;

class DevicePropertiesWidget : public QWidget
{
	Q_OBJECT

public:
	DevicePropertiesWidget(Device* device, QWidget* parent = nullptr);
	~DevicePropertiesWidget();

	Device* device() const { return pDevice; }

	void addPage(const QString& title, QWidget* page);

private:
	Device* pDevice = nullptr;
	QFrame* pFrameButtons = nullptr;
	QStackedWidget* pStackPages = nullptr;
	QVBoxLayout* pButtonLayout = nullptr;
	
};
