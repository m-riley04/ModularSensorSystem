#pragma once

#include <QWidget>
#include "ui_mountcontrolswidget.h"
#include <controllers/maincontroller.hpp>

class MountControlsWidget : public QWidget
{
	Q_OBJECT

public:
	MountControlsWidget(Mount* mount, MainController& mc, QWidget *parent = nullptr);
	~MountControlsWidget();

	Mount* mount() const { return m_mount; }

private slots:
	void updateUi();

	void onRefreshInfoClicked();
	void onSetInitialAnglesClicked();
	void onRecenterClicked();
	void onPanSliderChanged(int value);
	void onTiltSliderChanged(int value);

private:
	Ui::MountControlsWidgetClass ui;
	MainController& m_mainController;
	Mount* m_mount;
};

