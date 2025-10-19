#pragma once

#include <QDockWidget>
#include <QPointer>
#include "ui_dockableelementsmanagerwidget.h"
#include "controllers/maincontroller.h"
#include "models/ElementTreeModel/elementtreemodel.h"
#include <QMenu>
#include "data/requried_actions.h"

class DockableElementsManagerWidget : public QDockWidget
{
	Q_OBJECT

public:
	DockableElementsManagerWidget(QWidget *parent = nullptr);
	~DockableElementsManagerWidget();

	void setController(MainController* c);
	void setActions(ElementTreeActions actions);

private:
	Ui::DockableElementsManagerWidgetClass ui;
	QPointer<MainController> m_mainController;
	QPointer<ElementTreeModel> m_elementModel;

	void initWidgets();
	void initSignals();
	void initContextMenu();

	void handleRebuildClicked();
	void handleExpandAllClicked();
	void handleCollapseAllClicked();

	QPointer<QMenu> m_contextMenu;

	ElementTreeActions m_actions;

signals:
	void elementSelected();
};

