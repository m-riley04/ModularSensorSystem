#pragma once

#include <QDialog>
#include "ui_adddevicedialog.h"
#include <plugins/device/ideviceplugin.h>
#include <controllers/PluginController/plugincontroller.h>

class AddDeviceDialog : public QDialog
{
	Q_OBJECT

public:
	AddDeviceDialog(PluginController* pluginController, QWidget *parent = nullptr);
	~AddDeviceDialog();

private:
	Ui::AddDeviceDialogClass ui;
	QPointer<PluginController> pPluginController;

	void populateDeviceTypeDropdown();
	void populateDeviceDropdown();

signals:
	void deviceSelected(DeviceInfo* deviceInfo);
	void deviceTypeSelected(Device::Type type);
};
