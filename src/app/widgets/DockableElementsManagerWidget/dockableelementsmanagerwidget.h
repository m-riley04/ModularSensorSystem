#pragma once

#include <QDockWidget>
#include <QPointer>
#include <QMenu>
#include "ui_dockableelementsmanagerwidget.h"
#include "controllers/maincontroller.hpp"
#include "models/ElementTreeModel/elementtreemodel.h"
#include <controllers/AppActionController/appactioncontroller.h>

class DockableElementsManagerWidget : public QDockWidget
{
	Q_OBJECT

public:
	DockableElementsManagerWidget(QWidget *parent = nullptr);
	~DockableElementsManagerWidget();

	void setController(MainController* c);
	void setActions(AppActions* actions);

	ElementTreeNode* selectedNode() const { return m_selectedNode; }

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

	ElementTreeNode* m_selectedNode = nullptr;

	AppActions* m_actions;

public slots:
	void handleRebuildClicked();
	void handleExpandAllClicked();
	void handleCollapseAllClicked();

	void handleElementClicked(const QModelIndex& index);
	void handleRemoveElementClicked();
	void handleEditElementClicked();

private slots:
	void onElementSelected(const QModelIndex& current, const QModelIndex& newIdx);
	void onCustomContextMenuRequested(const QPoint& pos);

signals:
	void elementSelected(ElementTreeNode* node);
	void elementRemoved();
};

