#include "automationdialog.h"

#include <app/models/AutomationRulesListModel/automationruleslistmodel.h>
#include <widgets/AutomationRuleItemDelegate/automationruleitemdelegate.h>

#include <QAbstractItemView>

void AutomationDialog::populateRuleList()
{
	if (m_rulesModel) m_rulesModel->rebuild();
}

void AutomationDialog::onAddRule()
{
	if (!m_rulesModel) return;

	const int row = m_rulesModel->rowCount();
	m_rulesModel->insertRow(row);
	ui.listView->setCurrentIndex(m_rulesModel->index(row, 0));
	ui.listView->edit(m_rulesModel->index(row, 0));
}

void AutomationDialog::onRemoveRule()
{
	if (!m_rulesModel) return;
	const QModelIndex idx = ui.listView->currentIndex();
	if (!idx.isValid()) return;
	m_rulesModel->removeRow(idx.row());
}

AutomationDialog::AutomationDialog(RulesController& rc, QWidget *parent)
	: QDialog(parent), m_rulesController(rc)
{
	ui.setupUi(this);

	m_rulesModel = new AutomationRulesListModel(m_rulesController, this);
	ui.listView->setModel(m_rulesModel);
	ui.listView->setItemDelegate(new AutomationRuleItemDelegate(ui.listView));
	ui.listView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
	ui.listView->setSelectionMode(QAbstractItemView::SingleSelection);

	connect(ui.actionButtonAdd, &QPushButton::clicked, this, &AutomationDialog::onAddRule);
	connect(ui.actionButtonRemove, &QPushButton::clicked, this, &AutomationDialog::onRemoveRule);

	populateRuleList();


}

AutomationDialog::~AutomationDialog()
{}

