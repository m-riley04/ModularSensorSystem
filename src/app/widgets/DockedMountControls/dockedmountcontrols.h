#pragma once

#include <QDockWidget>
#include "ui_dockedmountcontrols.h"
#include "features/mounts/mount.hpp"

class DockedMountControls : public QDockWidget
{
	Q_OBJECT

public:
	DockedMountControls(QWidget *parent = nullptr);
	~DockedMountControls();

	Mount* mount() const { return m_mount; }
	void setMount(Mount* mount);

private slots:
	void updateUi();

	void onRefreshInfoClicked();
	void onSetInitialAnglesClicked();
	void onRecenterClicked();
	void onPanSliderChanged(int value);
	void onTiltSliderChanged(int value);

private:
	Ui::DockedMountControlsClass ui;
	Mount* m_mount = nullptr;
};

