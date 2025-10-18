#include "dockableelementsmanagerwidget.h"

DockableElementsManagerWidget::DockableElementsManagerWidget(QWidget *parent)
	: QDockWidget(parent)
{
	ui.setupUi(this);

	
}

DockableElementsManagerWidget::~DockableElementsManagerWidget()
{}

void DockableElementsManagerWidget::initWidgets()
{
	// Check if main controller is valid
	if (!pMainController) {
		return;
	}

	// Clean up old model
	if (pElementModel) {
		ui.treeElements->setModel(nullptr);
		pElementModel->deleteLater();
		pElementModel = nullptr;
	}

	// Create and set new model
	pElementModel = new ElementTreeModel(
		pMainController ? pMainController->mountController() : nullptr,
		pMainController ? pMainController->sourceController() : nullptr,
		pMainController ? pMainController->processingController() : nullptr,
		this);

	ui.treeElements->setModel(pElementModel);

	// Set the contents widget
	this->setWidget(ui.contents);
}

void DockableElementsManagerWidget::initSignals() {

	if (!pMainController) {
		return;
	}
	disconnect(ui.buttonRebuild, &QPushButton::clicked, pElementModel, &ElementTreeModel::rebuild);
	connect(ui.buttonRebuild, &QPushButton::clicked, pElementModel, &ElementTreeModel::rebuild);
}

