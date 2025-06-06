#pragma once

#include <QWidget>
#include "ui_processorswidget.h"
#include <dialogs/AddProcessorDialog/addprocessordialog.h>
#include <controllers/MainController/maincontroller.h>

class ProcessorsWidget : public QWidget
{
	Q_OBJECT

public:
	ProcessorsWidget(QWidget *parent = nullptr);
	~ProcessorsWidget();

	MainController* getController() const { return pController; }

	void setController(MainController* controller) {
		if (controller == pController) return;
		pController = controller;
		initSignals();
	}

private:
	Ui::ProcessorsWidgetClass ui;
	MainController* pController = nullptr;

	void initSignals();

public slots:
	void onProcessorAdded(ProcessorBase* processor); // Called when processor is added to/from controller
	void onProcessorRemoved(ProcessorBase* processor); // Called when processor is removed from/by controller

private slots:
	void onEnabledChanged(bool enabled);
	void onAddProcessorClicked();
	void onRemoveProcessorClicked();
	void onProcessorSelected(QListWidgetItem*, QListWidgetItem*);
	void onProcessingStarted();
	void onProcessingStopped();
	void onProcessingError(const QString& errorMessage);
};

