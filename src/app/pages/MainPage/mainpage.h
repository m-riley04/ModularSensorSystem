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

	QGroupBox* presetsGroup() const { return ui.groupPresets; }
	QGroupBox* sourcesGroup() const { return ui.groupSources; }
	QGroupBox* processorsGroup() const { return ui.groupProcessors; }
	QGroupBox* controlsGroup() const { return ui.groupControls; }

	PresetsWidget* presetsWidget() const { return ui.presetsWidget; }
	SourcesWidget* sourcesWidget() const { return ui.sourcesWidget; }
	ProcessorsWidget* processorsWidget() const { return ui.processorsWidget; }
	SessionControlsWidget* sessionControlsWidget() const { return ui.sessionControls; }

	QFrame* controlsFrame() const { return ui.frameControls; }

private:
	Ui::MainPageClass ui;

	MainController* pController = nullptr;

	void initWidgets();
	void initSignals();
};
