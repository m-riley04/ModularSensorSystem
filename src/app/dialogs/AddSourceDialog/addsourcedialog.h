#pragma once

#include <QDialog>
#include "ui_addsourcedialog.h"
#include "interfaces/plugins/isourceplugin.h"
#include "controllers/plugincontroller.h"
#include <QMessageBox>

class AddSourceDialog : public QDialog
{
	Q_OBJECT

public:
	AddSourceDialog(PluginController* pluginController, QWidget *parent = nullptr);
	~AddSourceDialog();

private:
	Ui::AddSourceDialogClass ui;
	QPointer<PluginController> pPluginController;

	ISourcePlugin* pSelectedSourcePlugin;
	SourceInfo mSelectedSourceInfo;

	void populateSourceTypeDropdown();
	void populateSourceDropdown();

signals:
	void sourceTypeSelected(ISourcePlugin* plugin);
	void sourceSelected(ISourcePlugin* plugin, SourceInfo deviceInfo);
	void sourceConfirmed(ISourcePlugin* plugin, SourceInfo deviceInfo);
	
};
