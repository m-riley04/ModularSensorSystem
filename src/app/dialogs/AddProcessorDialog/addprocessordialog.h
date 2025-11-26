#pragma once

#include <QDialog>
#include <qmessagebox.h>
#include <QPointer>
#include "ui_addprocessordialog.h"
#include "controllers/plugincontroller.hpp"

class AddProcessorDialog : public QDialog
{
	Q_OBJECT

public:
	AddProcessorDialog(PluginController* pluginController, QWidget *parent = nullptr);
	~AddProcessorDialog();

private:
	Ui::AddProcessorDialogClass ui;
	QPointer<PluginController> m_pluginController;

	IProcessorPlugin* pSelectedProcessorPlugin;
	ProcessorInfo mSelectedProcessor;

	void populateProcessorsDropdown();

private slots:
	void onProcessorSelected(int index);
	void onConfirmButtonClicked();
	void onCancelButtonClicked();

signals:
	void processorSelected(IProcessorPlugin* plugin);
	void processorConfirmed(IProcessorPlugin* plugin);
};

