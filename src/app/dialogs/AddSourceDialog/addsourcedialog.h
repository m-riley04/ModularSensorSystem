#pragma once

#include <QDialog>
#include <QMessageBox>
#include "ui_addsourcedialog.h"
#include <sdk/plugins/isourceplugin.hpp>
#include <controllers/plugincontroller.hpp>
#include <QPointer>

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
