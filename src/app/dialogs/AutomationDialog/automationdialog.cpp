#include "automationdialog.h"
#include <app/models/AutomationRulesListModel/automationruleslistmodel.h>
#include <widgets/AutomationRuleItemDelegate/automationruleitemdelegate.h>
#include <widgets/AutomationRuleItemWidget/automationruleitemwidget.h>

#include <widgets/widgets/GroupSelectWidget/groupselectwidget.h>

void AutomationDialog::populateRuleList()
{
	if (!m_rulesModel) return;

	// Keep existing widgets; rebuilding resets the model to controller state and can
	// overwrite recent edits before they are reflected.

	// Always-on editor widgets
	for (int row = 0; row < m_rulesModel->rowCount(); ++row) {
		QModelIndex idx = m_rulesModel->index(row, 0);
		auto* existing = ui.listView->indexWidget(idx);
		auto* w = qobject_cast<AutomationRuleItemWidget*>(existing);
		if (!w) {
			w = new AutomationRuleItemWidget(m_elementsController, m_sessionController, ui.listView);
			ui.listView->setIndexWidget(idx, w);
			connect(w, &AutomationRuleItemWidget::editingFinished, this, [this, row, w]() {
			// Persist via model custom roles
			if (auto* enabled = w->findChild<QCheckBox*>("checkboxToggleRule")) {
				m_rulesModel->setData(m_rulesModel->index(row, 0), enabled->isChecked() ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
			}
			m_rulesModel->rebuild(); // Refresh the model's internal rule list
			if (auto* actions = dynamic_cast<GroupSelectWidget*>(w->findChild<QWidget*>("selectActions"))) {
				const auto sel = actions->selectedValues();
				const QString v = sel.isEmpty() ? QString() : sel.first().userData.toString();
				// If it's not a known enum action (e.g. "mount.moveTo"), setData may reject it.
				// In that case, persist it via EditRole (description) is wrong; instead, bypass model
				// validation by storing the raw string in the model/controller if supported.
				if (!m_rulesModel->setData(m_rulesModel->index(row, 0), v, AutomationRulesListModel::Roles::ActionTypeRole)) {
					// Last resort: keep it in ActionTargetRole-like storage? No.
					// If the backend doesn't support custom actions yet, do nothing.
				}
			}
			if (auto* targets = dynamic_cast<GroupSelectWidget*>(w->findChild<QWidget*>("selectActionTargets"))) {
				const auto sel = targets->selectedValues();
				QStringList vals;
				vals.reserve(sel.size());
				for (const auto& o : sel) {
					const QString v = o.userData.toString();
					vals.append(v == "(Session)" ? QString() : v);
				}
				// Preserve explicit Session selection as an empty-token marker so it round-trips.
				const QString persisted = vals.isEmpty() ? QString() : vals.join(';');
				m_rulesModel->setData(m_rulesModel->index(row, 0), persisted, AutomationRulesListModel::Roles::ActionTargetRole);
			}
			if (auto* sources = dynamic_cast<GroupSelectWidget*>(w->findChild<QWidget*>("selectEventSources"))) {
				const auto sel = sources->selectedValues();
				QStringList vals;
				vals.reserve(sel.size());
				for (const auto& o : sel) {
					const QString v = o.userData.toString();
					vals.append(v == "(Session)" ? QString() : v);
				}
				const QString persisted = vals.isEmpty() ? QString() : vals.join(';');
				m_rulesModel->setData(m_rulesModel->index(row, 0), persisted, AutomationRulesListModel::Roles::TriggerConditionRole);
			}
			if (auto* types = dynamic_cast<GroupSelectWidget*>(w->findChild<QWidget*>("selectEventTypes"))) {
				const auto sel = types->selectedValues();
				m_rulesModel->setData(m_rulesModel->index(row, 0), sel.isEmpty() ? QString() : sel.first().userData.toString(), AutomationRulesListModel::Roles::TriggerTypeRole);
			}

			// Pull latest rule state back from controller to keep UI/model in sync.
			m_rulesModel->rebuild();
			w->setRule(m_rulesModel->ruleAt(row));
			});
		}

		// Refresh widget view from current model state
		w->setRule(m_rulesModel->ruleAt(row));
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

