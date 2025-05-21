#pragma once

#include <QDialog>
#include "ui_adddevicedialog.h"
#include <plugins/device/ideviceplugin.h>
#include <controllers/PluginController/plugincontroller.h>
#include <QMessageBox>

class AddDeviceDialog : public QDialog
{
	Q_OBJECT

public:
	AddDeviceDialog(PluginController* pluginController, QWidget *parent = nullptr);
	~AddDeviceDialog();

private:
	Ui::AddDeviceDialogClass ui;
	QPointer<PluginController> pPluginController;

	IDevicePlugin* pSelectedDevicePlugin;
	DeviceInfo mSelectedDeviceInfo;

	void populateDeviceTypeDropdown();
	void populateDeviceDropdown();

signals:
	void deviceTypeSelected(IDevicePlugin* plugin);
	void deviceSelected(IDevicePlugin* plugin, DeviceInfo deviceInfo);
	void deviceConfirmed(IDevicePlugin* plugin, DeviceInfo deviceInfo);
	
};
