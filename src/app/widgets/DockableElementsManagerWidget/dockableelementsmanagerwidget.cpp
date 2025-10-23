#include "dockableelementsmanagerwidget.h"

DockableElementsManagerWidget::DockableElementsManagerWidget(QWidget *parent)
	: QDockWidget(parent), m_actions(ElementTreeActions())
{
	ui.setupUi(this);

	
}

DockableElementsManagerWidget::~DockableElementsManagerWidget()
{}

void DockableElementsManagerWidget::setController(MainController* c)
{
	if (m_mainController == c) return;
	m_mainController = c;

	initWidgets();
	initSignals();
	initContextMenu();
}

void DockableElementsManagerWidget::setActions(ElementTreeActions actions)
{
	m_actions = ElementTreeActions(actions);

	initContextMenu();
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

	// TODO: consider disconnecting before reconnecting to avoid duplicate connections
	// From my minimal research, Qt should handle this automatically, but it's worth verifying.

	connect(m_mainController->sourceController(), &SourceController::sourceAdded, m_elementModel, &ElementTreeModel::rebuild);

	connect(m_mainController->mountController(), &MountController::mountAdded, m_elementModel, &ElementTreeModel::rebuild);

	connect(m_mainController->processingController(), &ProcessingController::processorAdded, m_elementModel, &ElementTreeModel::rebuild);

	connect(ui.treeElements, &QTreeView::clicked, this, &DockableElementsManagerWidget::handleElementClicked);
}

void DockableElementsManagerWidget::initContextMenu()
{
	if (m_contextMenu) {
		m_contextMenu->deleteLater();
		m_contextMenu = nullptr;
	}

	m_contextMenu = new QMenu(this);

	QMenu* pAddElementMenu = m_contextMenu->addMenu("Add...");
	pAddElementMenu->addAction(m_actions.addMount);
	pAddElementMenu->addAction(m_actions.addSource);
	pAddElementMenu->addAction(m_actions.addProcessor);

	m_contextMenu->addSeparator();

	m_actionRemoveElement = m_contextMenu->addAction("Remove", this, &DockableElementsManagerWidget::handleRemoveElementClicked);
	/*m_contextMenu->addAction(m_actions.removeMount);
	m_contextMenu->addAction(m_actions.removeProcessor);
	m_contextMenu->addAction(m_actions.removeSource);*/

	m_actionEditElement = m_contextMenu->addAction("Edit", this, &DockableElementsManagerWidget::handleEditElementClicked);
	/*m_contextMenu->addAction(m_actions.editMount);
	m_contextMenu->addAction(m_actions.editProcessor);
	m_contextMenu->addAction(m_actions.editSource);*/

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

void DockableElementsManagerWidget::handleElementClicked(const QModelIndex& index)
{
	qDebug() << "Element clicked:" << index;

	if (!index.isValid() || !m_elementModel) {
		qDebug() << "Invalid index or model.";
		return;
	}
	
	QVariant nodeData = m_elementModel->data(index, Qt::UserRole);
	if (!nodeData.isValid() || !nodeData.canConvert<ElementTreeNode>()) {
		qDebug() << "Invalid node data.";
		return;
	}

	qDebug() << "Node data:" << nodeData;

	// Update the selected node
	m_selectedNode = nodeData.value<ElementTreeNode>();
	emit elementSelected(&m_selectedNode);
}

void DockableElementsManagerWidget::handleRemoveElementClicked()
{
	switch (m_selectedNode.kind) {
	case ElementTreeNode::Kind::Mount:
		m_actions.removeMount->trigger();
		break;
	case ElementTreeNode::Kind::Source:
		m_actions.removeSource->trigger();
		break;
	case ElementTreeNode::Kind::Processor:
		m_actions.removeProcessor->trigger();
		break;
	case ElementTreeNode::Kind::None:
	default:
		break;
	}
}

void DockableElementsManagerWidget::handleEditElementClicked()
{
	switch (m_selectedNode.kind) {
	case ElementTreeNode::Kind::Mount:
		m_actions.editMount->trigger();
		break;
	case ElementTreeNode::Kind::Source:
		m_actions.editSource->trigger();
		break;
	case ElementTreeNode::Kind::Processor:
		m_actions.editProcessor->trigger();
		break;
	case ElementTreeNode::Kind::None:
	default:
		break;
	}
}

