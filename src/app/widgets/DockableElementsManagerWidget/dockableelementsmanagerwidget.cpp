#include "dockableelementsmanagerwidget.h"
#include "models/ElementTreeModel/elementtreemodel.h"

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
	pElementModel = new ElementTreeModel(pMainController, this);

	ui.treeElements->setModel(pElementModel);

	// Set the contents widget
	this->setWidget(ui.contents);
}

void DockableElementsManagerWidget::initSignals() {

	if (!pMainController) {
		return;
	}
	disconnect(pMainController->sourceController(), &SourceController::sourceAdded, pElementModel, &ElementTreeModel::rebuild);
	connect(pMainController->sourceController(), &SourceController::sourceAdded, pElementModel, &ElementTreeModel::rebuild);


	disconnect(pMainController->mountController(), &MountController::mountAdded, pElementModel, &ElementTreeModel::rebuild);
	connect(pMainController->mountController(), &MountController::mountAdded, pElementModel, &ElementTreeModel::rebuild);

	disconnect(pMainController->processingController(), &ProcessingController::processorAdded, pElementModel, &ElementTreeModel::rebuild);
	connect(pMainController->processingController(), &ProcessingController::processorAdded, pElementModel, &ElementTreeModel::rebuild);

	disconnect(ui.buttonRebuild, &QPushButton::clicked, pElementModel, &ElementTreeModel::rebuild);
	connect(ui.buttonRebuild, &QPushButton::clicked, pElementModel, &ElementTreeModel::rebuild);
}

