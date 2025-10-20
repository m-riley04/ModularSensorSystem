#pragma once

#include <QWidget>
#include "ui_mainpage.h"
#include "controllers/maincontroller.h"

class MainPage : public QMainWindow
{
	Q_OBJECT

public:
	MainPage(MainController* controller, ElementTreeActions actions, QWidget *parent = nullptr);
	~MainPage();

	QGroupBox* presetsGroup() const { return ui.groupPresets; }
	QGroupBox* controlsGroup() const { return ui.groupControls; }

	PresetsWidget* presetsWidget() const { return ui.presetsWidget; }
	SessionControlsWidget* sessionControlsWidget() const { return ui.sessionControls; }
	DockableElementsManagerWidget* elementsTreeWidget() const { return ui.dockWidget; }

	QFrame* controlsFrame() const { return ui.frameControls; }

private:
	Ui::MainPageClass ui;

	MainController* pController = nullptr;
	ElementTreeActions m_actions;

	void initWidgets();
	void initSignals();
};
