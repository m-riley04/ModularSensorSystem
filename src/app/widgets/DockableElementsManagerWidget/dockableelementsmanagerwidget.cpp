#include "dockableelementsmanagerwidget.h"
#include "windows/MainWindow/MainWindow.h"

DockableElementsManagerWidget::DockableElementsManagerWidget(QWidget *parent)
	: QDockWidget(parent)
{
	ui.setupUi(this);

	
}

DockableElementsManagerWidget::~DockableElementsManagerWidget()
{}

void DockableElementsManagerWidget::setActions(QAction* addMount, QAction* addSource, QAction* addProcessor)
{
	m_actionAddMount = addMount;
	m_actionAddSource = addSource;
	m_actionAddProcessor = addProcessor;

	initContextMenu();
}

void DockableElementsManagerWidget::setMainWindow(MainWindow* mw)
{
	m_mainWindow = mw;

	setActions(mw->actionAddMount(), mw->actionAddSource(), mw->actionAddProcessor());
}

void DockableElementsManagerWidget::initWidgets()
{
	// Check if main controller is valid
	if (!m_mainController) {
		return;
	}

	// Clean up old model
	if (m_elementModel) {
		ui.treeElements->setModel(nullptr);
		m_elementModel->deleteLater();
		m_elementModel = nullptr;
	}

	// Create and set new model
	m_elementModel = new ElementTreeModel(m_mainController, this);

	ui.treeElements->setModel(m_elementModel);

	// Set the contents widget
	this->setWidget(ui.contents);
}

void DockableElementsManagerWidget::initSignals() {

	if (!m_mainController) {
		return;
	}
	disconnect(m_mainController->sourceController(), &SourceController::sourceAdded, m_elementModel, &ElementTreeModel::rebuild);
	connect(m_mainController->sourceController(), &SourceController::sourceAdded, m_elementModel, &ElementTreeModel::rebuild);

	disconnect(m_mainController->mountController(), &MountController::mountAdded, m_elementModel, &ElementTreeModel::rebuild);
	connect(m_mainController->mountController(), &MountController::mountAdded, m_elementModel, &ElementTreeModel::rebuild);

	disconnect(m_mainController->processingController(), &ProcessingController::processorAdded, m_elementModel, &ElementTreeModel::rebuild);
	connect(m_mainController->processingController(), &ProcessingController::processorAdded, m_elementModel, &ElementTreeModel::rebuild);

	disconnect(ui.buttonRebuild, &QPushButton::clicked, m_elementModel, &ElementTreeModel::rebuild);
	connect(ui.buttonRebuild, &QPushButton::clicked, m_elementModel, &ElementTreeModel::rebuild);
}

void DockableElementsManagerWidget::initContextMenu()
{
	if (m_contextMenu) {
		m_contextMenu->deleteLater();
		m_contextMenu = nullptr;
	}

	m_contextMenu = new QMenu(this);

	QMenu* pAddElementMenu = m_contextMenu->addMenu("Add Element");
	pAddElementMenu->addAction(m_actionAddMount);
	pAddElementMenu->addAction(m_actionAddSource);
	pAddElementMenu->addAction(m_actionAddProcessor);

	m_contextMenu->addAction("Rebuild", this, &DockableElementsManagerWidget::handleRebuildClicked);

	m_contextMenu->addSeparator();

	m_contextMenu->addAction("Expand All", this, &DockableElementsManagerWidget::handleExpandAllClicked);

	m_contextMenu->addAction("Collapse All", this, &DockableElementsManagerWidget::handleCollapseAllClicked);

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &QDockWidget::customContextMenuRequested, this, [this](const QPoint& pos) {
		m_contextMenu->exec(mapToGlobal(pos));
	});
}

void DockableElementsManagerWidget::handleRebuildClicked()
{
	if (m_elementModel) {
		m_elementModel->rebuild();
	}
}

void DockableElementsManagerWidget::handleExpandAllClicked()
{
	if (ui.treeElements) {
		ui.treeElements->expandAll();
	}
}

void DockableElementsManagerWidget::handleCollapseAllClicked()
{
	if (ui.treeElements) {
		ui.treeElements->collapseAll();
	}
}

