#include "dockableelementsmanagerwidget.h"
#include <qmessagebox.h>

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
	m_actions = actions;

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
	m_elementModel = new ElementTreeModel(*m_mainController, this);

	ui.treeElements->setModel(m_elementModel);

	// Set the contents widget
	this->setWidget(ui.contents);

	// Ensure right-click selects the item under cursor by handling the tree view's context menu
	ui.treeElements->setContextMenuPolicy(Qt::CustomContextMenu);
}

void DockableElementsManagerWidget::initSignals() {
	connect(ui.treeElements->selectionModel(), &QItemSelectionModel::currentChanged, this, &DockableElementsManagerWidget::onElementSelected);
	connect(ui.treeElements, &QTreeView::customContextMenuRequested, this, &DockableElementsManagerWidget::onCustomContextMenuRequested);
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
	m_actionEditElement = m_contextMenu->addAction("Edit", this, &DockableElementsManagerWidget::handleEditElementClicked);

	m_contextMenu->addSeparator();

	m_contextMenu->addAction("Rebuild", this, &DockableElementsManagerWidget::handleRebuildClicked);

	m_contextMenu->addAction("Expand All", this, &DockableElementsManagerWidget::handleExpandAllClicked);

	m_contextMenu->addAction("Collapse All", this, &DockableElementsManagerWidget::handleCollapseAllClicked);

	// Context menu is handled on the tree view for accurate item selection
}

void DockableElementsManagerWidget::onElementSelected(const QModelIndex& currentIdx, const QModelIndex& prevIdx) {
	if (!currentIdx.isValid() || !m_elementModel) {
		qDebug() << "Invalid index or model.";
		m_actionRemoveElement->setEnabled(false);
		m_actionEditElement->setEnabled(false);
		return;
	}

	QVariant nodeData = m_elementModel->data(currentIdx, Qt::UserRole);
	if (!nodeData.isValid() || !nodeData.canConvert<ElementTreeNode*>()) {
		qDebug() << "Invalid node data.";
		m_actionRemoveElement->setEnabled(false);
		m_actionEditElement->setEnabled(false);
		return;
	}

	qDebug() << "Node data:" << nodeData;

	ui.treeElements->setCurrentIndex(currentIdx);

	// Update the selected node
	m_selectedNode = nodeData.value<ElementTreeNode*>();

	m_actionRemoveElement->setEnabled(true);
	m_actionEditElement->setEnabled(true);

	emit elementSelected(m_selectedNode);
}

void DockableElementsManagerWidget::handleRebuildClicked()
{
	m_elementModel->rebuild();
}

void DockableElementsManagerWidget::handleExpandAllClicked()
{
	ui.treeElements->expandAll();
}

void DockableElementsManagerWidget::handleCollapseAllClicked()
{
	ui.treeElements->collapseAll();
}

void DockableElementsManagerWidget::handleElementClicked(const QModelIndex& index)
{
	auto previousIndex = ui.treeElements->currentIndex();
	onElementSelected(index, previousIndex);
}

void DockableElementsManagerWidget::handleRemoveElementClicked()
{
	if (!m_selectedNode) return;

	switch (m_selectedNode->kind) {
	case IElement::Type::Mount:
		m_actions.removeMount->trigger();
		break;
	case IElement::Type::Source:
		m_actions.removeSource->trigger();
		break;
	case IElement::Type::Processor:
		m_actions.removeProcessor->trigger();
		break;
	case IElement::Type::Unknown:
	default:
		break;
	}

	this->update();

	emit elementRemoved();
}

void DockableElementsManagerWidget::handleEditElementClicked()
{
	if (!m_selectedNode) return;

	switch (m_selectedNode->kind) {
	case IElement::Type::Mount:
		m_actions.editMount->trigger();
		break;
	case IElement::Type::Source:
		m_actions.editSource->trigger();
		break;
	case IElement::Type::Processor:
		m_actions.editProcessor->trigger();
		break;
	case IElement::Type::Unknown:
	default:
		break;
	}

	this->update();
}

void DockableElementsManagerWidget::onCustomContextMenuRequested(const QPoint& pos)
{
	QModelIndex index = ui.treeElements->indexAt(pos);
	auto previousIndex = ui.treeElements->currentIndex();
	onElementSelected(index, previousIndex);
	if (m_contextMenu) {
		m_contextMenu->exec(ui.treeElements->viewport()->mapToGlobal(pos));
	}
}

