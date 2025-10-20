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

	QPointer<QAction> m_actionRemoveElement;
	QPointer<QAction> m_actionEditElement;

	QPointer<QMenu> m_contextMenu;

	Node m_selectedNode;

	ElementTreeActions m_actions;

public slots:
	void handleRebuildClicked();
	void handleExpandAllClicked();
	void handleCollapseAllClicked();

	void handleElementClicked(const QModelIndex& index);
	void handleRemoveElementClicked();
	void handleEditElementClicked();

signals:
	void elementSelected();
};

