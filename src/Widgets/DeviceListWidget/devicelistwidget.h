#pragma once

#include <QWidget>
#include "ui_devicelistwidget.h"

#include "Windows/AddCameraDialog/AddCameraDialog.h"
#include <Controllers/DeviceController/devicecontroller.h>

class DeviceListWidget : public QWidget
{
	Q_OBJECT

public:
	DeviceListWidget(QWidget* parent = nullptr);
	DeviceListWidget(DeviceController* deviceController, QWidget *parent = nullptr);
	~DeviceListWidget();

private:
	Ui::DeviceListWidgetClass ui;
	DeviceController* pDeviceController;

	void initSignals();
	void initDeviceControllerSignals();

public slots:
	void openAddDeviceDialog();
	void openRemoveDeviceDialog();
	void setDeviceController(DeviceController* deviceController);

signals:
	void deviceControllerChanged(DeviceController* deviceController);
};
