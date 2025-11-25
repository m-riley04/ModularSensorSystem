#pragma once

#include <QDialog>
#include <qmessagebox.h>
#include "ui_addmountdialog.h"
#include <sdk/plugins/imountplugin.hpp>
#include <controllers/plugincontroller.hpp>
#include <QPointer>

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
	ElementInfo m_selectedMountInfo{};

	void populateMountTypeDropdown();
	void populateMountDropdown();

signals:
	void mountTypeSelected(IMountPlugin* plugin);
	void mountSelected(IMountPlugin* plugin, ElementInfo mount);
	void mountConfirmed(IMountPlugin* plugin, ElementInfo mount);
};
