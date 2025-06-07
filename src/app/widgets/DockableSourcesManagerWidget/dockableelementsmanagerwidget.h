#pragma once

#include <QDockWidget>
#include "ui_dockablesourcesmanagerwidget.h"
#include <controllers/MainController/maincontroller.h>

class DockableElementsManagerWidget : public QDockWidget
{
	Q_OBJECT

public:
	DockableElementsManagerWidget(QWidget *parent = nullptr);
	~DockableElementsManagerWidget();

	void setController(MainController* c) {
		if (pMainController == c) return;
		pMainController = c;
	}

private:
	Ui::DockableSourcesManagerWidgetClass ui;
	QPointer<MainController> pMainController;

	void initWidgets();
	void initSignals();

signals:
	void elementSelected();
};

