#pragma once

#include <QDialog>
#include <qmessagebox.h>
#include <QPointer>
#include "ui_addprocessordialog.h"
#include "controllers/plugincontroller.hpp"
#include <controllers/elementscontroller.hpp>

class AddProcessorDialog : public QDialog
{
	Q_OBJECT

public:
	AddProcessorDialog(PluginController& pc, ElementsController& ec, QWidget *parent = nullptr);
	~AddProcessorDialog();

private:
	Ui::AddProcessorDialogClass ui;
	PluginController& m_pluginController;
	ElementsController& m_elementsController;

	IProcessorPlugin* pSelectedProcessorPlugin;
	ProcessorInfo mSelectedProcessor;
	Source* m_selectedSource;

	void populateProcessorsDropdown();
	void populateSourcesDropdown();

private slots:
	void onProcessorSelected(int index);
	void onSourceSelected(int index);
	void onConfirmButtonClicked();
	void onCancelButtonClicked();

	void onRefreshProcessorsButtonClicked();
	void onRefreshSourcesButtonClicked();

signals:
	void sourceSelected(Source* source);
	void processorSelected(IProcessorPlugin* plugin);
	void processorConfirmed(IProcessorPlugin* plugin, Source* source);
};

