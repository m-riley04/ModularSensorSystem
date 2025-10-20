#pragma once

#include <QWidget>
#include "ui_mainpage.h"
#include "controllers/maincontroller.h"

class MainWindow;

class MainPage : public QMainWindow
{
	Q_OBJECT

public:
	MainPage(MainController* controller, MainWindow* mainWindow, QWidget *parent = nullptr);
	~MainPage();

	QGroupBox* presetsGroup() const { return ui.groupPresets; }
	QGroupBox* controlsGroup() const { return ui.groupControls; }

	PresetsWidget* presetsWidget() const { return ui.presetsWidget; }
	SessionControlsWidget* sessionControlsWidget() const { return ui.sessionControls; }

	QFrame* controlsFrame() const { return ui.frameControls; }

private:
	Ui::MainPageClass ui;

	MainController* pController = nullptr;
	MainWindow* m_mainWindow = nullptr;

	void initWidgets();
	void initSignals();
};
