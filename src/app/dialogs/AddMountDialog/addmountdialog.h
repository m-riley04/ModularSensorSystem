#pragma once

#include <QDialog>
#include "ui_addmountdialog.h"
#include <sdk/plugins/imountplugin.h>
#include <controllers/plugincontroller.h>
#include <qmessagebox.h>

class AddMountDialog : public QDialog
{
	Q_OBJECT

public:
	AddMountDialog(PluginController* pluginController, QWidget *parent = nullptr);
	~AddMountDialog() = default;

private:
	Ui::AddMountDialogClass ui;

	QPointer<PluginController> m_pluginController = nullptr;
	IMountPlugin* m_selectedMountPlugin = nullptr;
	MountInfo m_selectedMountInfo{};

	void populateMountTypeDropdown();
	void populateMountDropdown();

signals:
	void mountTypeSelected(IMountPlugin* plugin);
	void mountSelected(IMountPlugin* plugin, MountInfo mount);
	void mountConfirmed(IMountPlugin* plugin, MountInfo mount);
};
