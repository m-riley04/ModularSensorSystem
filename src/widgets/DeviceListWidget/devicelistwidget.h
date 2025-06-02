#pragma once

#include <QWidget>
#include "ui_devicelistwidget.h"
#include <qmessagebox.h>
#include "dialogs/AddDeviceDialog/adddevicedialog.h"
#include <Controllers/DeviceController/devicecontroller.h>
#include <devices/IConfigurableDevice/iconfigurabledevice.h>
#include <qobject.h>

class DeviceListWidget : public QWidget
{
	Q_OBJECT

public:
	DeviceListWidget(QWidget* parent = nullptr);
	DeviceListWidget(SourceController* sourceController, QWidget *parent = nullptr);
	~DeviceListWidget();

private:
	Ui::DeviceListWidgetClass ui;
	SourceController* pDeviceController;

	void initSignals();
	void initDeviceControllerSignals();

public slots:
	void openAddDeviceDialog();
	void openRemoveDeviceDialog();
	void openProperties();
	void setDeviceController(SourceController* sourceController);

signals:
	void deviceControllerChanged(SourceController* sourceController);
};
