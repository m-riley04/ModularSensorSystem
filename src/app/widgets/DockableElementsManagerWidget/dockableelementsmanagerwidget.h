#pragma once

#include <QDockWidget>
#include <QPointer>
#include "ui_dockableelementsmanagerwidget.h"
#include "controllers/maincontroller.h"
#include "models/ElementTreeModel/elementtreemodel.h"
#include <QMenu>

class MainWindow;


class DockableElementsManagerWidget : public QDockWidget
{
	Q_OBJECT

public:
	DockableElementsManagerWidget(QWidget *parent = nullptr);
	~DockableElementsManagerWidget();

	void setController(MainController* c) {
		if (m_mainController == c) return;
		m_mainController = c;

		initWidgets();
		initSignals();
	}

	void setActions(QAction* addMount, QAction* addSource, QAction* addProcessor);

	void setMainWindow(MainWindow* mw);

private:
	Ui::DockableElementsManagerWidgetClass ui;
	QPointer<MainController> m_mainController;
	QPointer<ElementTreeModel> m_elementModel;
	QPointer<MainWindow> m_mainWindow;

	void initWidgets();
	void initSignals();
	void initContextMenu();

	void handleRebuildClicked();
	void handleExpandAllClicked();
	void handleCollapseAllClicked();

	QPointer<QMenu> m_contextMenu;

	QPointer<QAction> m_actionAddMount;
	QPointer<QAction> m_actionAddSource;
	QPointer<QAction> m_actionAddProcessor;

signals:
	void elementSelected();
};

