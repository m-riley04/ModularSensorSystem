#pragma once

#include <QWidget>
#include "ui_mainpage.h"
#include <Controllers/MainController/maincontroller.h>

class MainPage : public QWidget
{
	Q_OBJECT

public:
	MainPage(MainController* controller, QWidget *parent = nullptr);
	~MainPage();

private:
	Ui::MainPageClass ui;

	MainController* pController = nullptr;

	void initWidgets();
	void initSignals();
};
