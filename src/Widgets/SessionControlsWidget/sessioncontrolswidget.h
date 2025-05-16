#pragma once

#include <QWidget>
#include "ui_sessioncontrolswidget.h"
#include <Controllers/MainController/maincontroller.h>

class SessionControlsWidget : public QWidget
{
	Q_OBJECT

public:
	SessionControlsWidget(QWidget *parent = nullptr);
	~SessionControlsWidget();

	void setController(MainController* controller) { 
		if (pController == controller) return;
		pController = controller;
		initSignals();
		updateUi();
	}

private:
	Ui::SessionControlsWidgetClass ui;

	MainController* pController = nullptr;

	void initSignals();
	
private slots:
	void updateUi();
};
