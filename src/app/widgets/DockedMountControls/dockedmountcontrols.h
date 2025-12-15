#pragma once

#include <QDockWidget>
#include <QPointer>
#include "ui_dockedmountcontrols.h"
#include "features/mounts/mount.hpp"
#include <controllers/maincontroller.hpp>

class DockedMountControls : public QDockWidget
{
	Q_OBJECT

public:
	DockedMountControls(QWidget *parent = nullptr);
	~DockedMountControls();

	void setController(MainController*);
	void setMountId(const QUuid& mountId);

private slots:
	void refreshMountIdInfo();

	void updateUi();
	void updatePanTiltUi();

	void onRefreshInfoClicked();
	void onSetInitialAnglesClicked();
	void onRecenterClicked();
	void onPanSliderChanged(int value);
	void onTiltSliderChanged(int value);

private:
	Ui::DockedMountControlsClass ui;
	QUuid m_mountId;
	QPointer<MainController> m_controller;
};

