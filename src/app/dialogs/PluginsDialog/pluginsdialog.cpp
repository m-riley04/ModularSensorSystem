#include "pluginsdialog.h"

PluginsDialog::PluginsDialog(PluginController& c, QWidget *parent)
	: QDialog(parent), m_pluginController(c)
{
	ui.setupUi(this);

	initModel();
	initWidgets();
	initSignals();
	initContextMenu();
}

PluginsDialog::~PluginsDialog()
{}

void PluginsDialog::initModel()
{
	// Clean up old model
	if (m_model) {
		ui.listPlugins->setModel(nullptr);
		m_model->deleteLater();
		m_model = nullptr;
	}

	// Create and set new model
	m_model = new PluginListModel(m_pluginController, this);
	ui.listPlugins->setModel(m_model);
}

void PluginsDialog::initWidgets()
{
	// Ensure right-click selects the item under cursor by handling the list view's context menu
	ui.listPlugins->setContextMenuPolicy(Qt::CustomContextMenu);
}

void PluginsDialog::initSignals() {
	connect(ui.listPlugins->selectionModel(), &QItemSelectionModel::currentChanged, this, &PluginsDialog::onPluginSelected);
	connect(ui.listPlugins, &QTreeView::customContextMenuRequested, this, &PluginsDialog::onCustomContextMenuRequested);
}

void PluginsDialog::initContextMenu()
{
	if (m_contextMenu) {
		m_contextMenu->deleteLater();
		m_contextMenu = nullptr;
	}

	m_contextMenu = new QMenu(this);

	m_contextMenu->addSeparator();

	m_actionTogglePlugin = m_contextMenu->addAction("Unload", this, &PluginsDialog::onToggleSelectedPlugin);

	m_contextMenu->addSeparator();

	m_contextMenu->addAction("Rescan", this, &PluginsDialog::onRescanPlugins);
}

void PluginsDialog::onPluginSelected(const QModelIndex& currentIdx, const QModelIndex& newIdx)
{
	if (!currentIdx.isValid() || !m_model) {
		qDebug() << "Invalid index or model.";
		m_actionTogglePlugin->setEnabled(false);
		return;
	}

	QVariant nodeData = m_model->data(currentIdx, Qt::UserRole);
	if (!nodeData.isValid() || !nodeData.canConvert<PluginMetadata*>()) {
		qDebug() << "Invalid node data.";
		m_actionTogglePlugin->setEnabled(false);
		return;
	}

	ui.listPlugins->setCurrentIndex(currentIdx);

	// Update the selected plugin
	m_selectedPlugin = nodeData.value<PluginMetadata*>();

	m_actionTogglePlugin->setEnabled(true);
	ui.pluginDetailsWidget->setPlugin(m_selectedPlugin);
}

void PluginsDialog::onCustomContextMenuRequested(const QPoint& pos)
{
	QModelIndex index = ui.listPlugins->indexAt(pos);
	auto previousIndex = ui.listPlugins->currentIndex();
	onPluginSelected(index, previousIndex);
	if (!m_contextMenu) return;
	m_contextMenu->exec(ui.listPlugins->viewport()->mapToGlobal(pos));
}

void PluginsDialog::onRescanPlugins()
{
	m_pluginController.rescanPlugins();
	initModel();
}

void PluginsDialog::onToggleSelectedPlugin()
{
	if (!m_selectedPlugin) return;

	// Check current state
	bool isLoaded = m_pluginController.registry().isLoaded(m_selectedPlugin->path);
	if (!isLoaded) {
		m_pluginController.loadPlugin(QString::fromStdString(m_selectedPlugin->path));
	} else {
		m_pluginController.unloadPlugin(QString::fromStdString(m_selectedPlugin->path));
	}

	m_actionTogglePlugin->setText(isLoaded ? "Load" : "Unload");
}