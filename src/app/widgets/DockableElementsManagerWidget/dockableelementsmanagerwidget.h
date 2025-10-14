#pragma once

#include <QDockWidget>
#include "ui_dockableelementsmanagerwidget.h"
#include "controllers/maincontroller.h"

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
	Ui::DockableElementsManagerWidgetClass ui;
	QPointer<MainController> pMainController;

	void initWidgets();
	void initSignals();

signals:
	void elementSelected();
};

