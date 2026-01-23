#include "automationdialog.h"

#include <app/models/AutomationRulesListModel/automationruleslistmodel.h>
#include <widgets/AutomationRuleItemDelegate/automationruleitemdelegate.h>
#include <widgets/AutomationRuleItemWidget/automationruleitemwidget.h>

#include <QAbstractItemView>

void AutomationDialog::populateRuleList()
{
	if (!m_rulesModel) return;
	m_rulesModel->rebuild();

	// Always-on editor widgets
	for (int row = 0; row < m_rulesModel->rowCount(); ++row) {
		QModelIndex idx = m_rulesModel->index(row, 0);
		auto* w = new AutomationRuleItemWidget(m_elementsController, m_sessionController, ui.listView);
		w->setRule(m_rulesModel->ruleAt(row));
		connect(w, &AutomationRuleItemWidget::editingFinished, this, [this, row, w]() {
			// Persist via model custom roles
			if (auto* cb = w->findChild<QComboBox*>("dropdownAction")) {
				m_rulesModel->setData(m_rulesModel->index(row, 0), cb->currentData(), AutomationRulesListModel::Roles::ActionTypeRole);
			}
			if (auto* cb = w->findChild<QComboBox*>("dropdownActionTarget")) {
				m_rulesModel->setData(m_rulesModel->index(row, 0), cb->currentData(), AutomationRulesListModel::Roles::ActionTargetRole);
			}
			if (auto* cb = w->findChild<QComboBox*>("dropdownConditionTarget")) {
				m_rulesModel->setData(m_rulesModel->index(row, 0), cb->currentData(), AutomationRulesListModel::Roles::TriggerConditionRole);
			}

			// Ensure the widget remains installed for this row (some view updates can clear it)
			const QModelIndex idx = m_rulesModel->index(row, 0);
			if (ui.listView->indexWidget(idx) != w) {
				ui.listView->setIndexWidget(idx, w);
			}
		});
		ui.listView->setIndexWidget(idx, w);
	}
}

void AutomationDialog::onAddRule()
{
	if (!m_rulesModel) return;

	const int row = m_rulesModel->rowCount();
	m_rulesModel->insertRow(row);
	populateRuleList();
}

void AutomationDialog::onRemoveRule()
{
	if (!m_rulesModel) return;
	// Remove last rule for now since we're not using selection in always-on widget mode.
	const int row = m_rulesModel->rowCount() - 1;
	if (row < 0) return;
	m_rulesModel->removeRow(row);
	populateRuleList();
}

AutomationDialog::AutomationDialog(RulesController& rc, ElementsController& ec, SessionController& sc, QWidget *parent)
	: QDialog(parent), m_rulesController(rc), m_elementsController(ec), m_sessionController(sc)
{
	ui.setupUi(this);

	m_rulesModel = new AutomationRulesListModel(m_rulesController, this);
	ui.listView->setModel(m_rulesModel);
	ui.listView->setItemDelegate(new AutomationRuleItemDelegate(m_elementsController, m_sessionController, ui.listView));
	ui.listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.listView->setSelectionMode(QAbstractItemView::NoSelection);

	connect(ui.actionButtonAdd, &QPushButton::clicked, this, &AutomationDialog::onAddRule);
	connect(ui.actionButtonRemove, &QPushButton::clicked, this, &AutomationDialog::onRemoveRule);

	populateRuleList();


}

AutomationDialog::~AutomationDialog()
{}

