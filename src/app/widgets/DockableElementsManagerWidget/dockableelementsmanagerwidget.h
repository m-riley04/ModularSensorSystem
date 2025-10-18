#pragma once

#include <QDockWidget>
#include "ui_dockableelementsmanagerwidget.h"
#include "controllers/maincontroller.h"
#include "models/ElementTreeModel/elementtreemodel.h"

class DockableElementsManagerWidget : public QDockWidget
{
	Q_OBJECT

public:
	DockableElementsManagerWidget(QWidget *parent = nullptr);
	~DockableElementsManagerWidget();

	void setController(MainController* c) {
		if (pMainController == c) return;
		pMainController = c;

		initWidgets();
		initSignals();
	}

private:
	Ui::DockableElementsManagerWidgetClass ui;
	QPointer<MainController> pMainController;
	QPointer<ElementTreeModel> pElementModel;

	void initWidgets();
	void initSignals();

signals:
	void elementSelected();
};

