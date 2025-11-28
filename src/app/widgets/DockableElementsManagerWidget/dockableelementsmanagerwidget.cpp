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


    initSignals();
}

void DockableElementsManagerWidget::initSignals() {

	if (!m_mainController) {
		return;
	}

	connect(ui.treeElements, &QTreeView::clicked, this, &DockableElementsManagerWidget::handleElementClicked);
	connect(ui.treeElements, &QTreeView::doubleClicked, [this]() {
		QMessageBox::information(this, "Info", "Double click action is not implemented yet.");
		});
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
	qDebug() << "Element clicked:" << index;

	if (!index.isValid() || !m_elementModel) {
		qDebug() << "Invalid index or model.";
		return;
	}
	
	QVariant nodeData = m_elementModel->data(index, Qt::UserRole);
	if (!nodeData.isValid() || !nodeData.canConvert<ElementTreeNode*>()) {
		qDebug() << "Invalid node data.";
		return;
	}

	qDebug() << "Node data:" << nodeData;

	// Update the selected node
	m_selectedNode = nodeData.value<ElementTreeNode*>();

	emit elementSelected(m_selectedNode);
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

