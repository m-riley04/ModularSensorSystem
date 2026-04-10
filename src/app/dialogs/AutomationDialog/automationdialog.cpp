#include "automationdialog.h"
#include <widgets/AutomationRuleItemWidget/automationruleitemwidget.h>
#include <widgets/widgets/GroupSelectWidget/groupselectwidget.h>

#include <QScrollArea>
#include <QVBoxLayout>

AutomationDialog::AutomationDialog(RulesController& rc, ElementsController& ec, SessionController& sc, QWidget* parent)
	: QDialog(parent), m_rulesController(rc), m_elementsController(ec), m_sessionController(sc)
{
	ui.setupUi(this);

	// Replace the listView with a simple scroll area + vertical layout.
	auto* scrollArea = new QScrollArea(this);
	scrollArea->setWidgetResizable(true);

	auto* container = new QWidget(scrollArea);
	m_rulesLayout = new QVBoxLayout(container);
	m_rulesLayout->setAlignment(Qt::AlignTop);
	m_rulesLayout->setContentsMargins(4, 4, 4, 4);
	m_rulesLayout->setSpacing(4);
	scrollArea->setWidget(container);

	// Swap the listView out of the parent layout
	if (auto* parentLayout = ui.listView->parentWidget()->layout()) {
		parentLayout->replaceWidget(ui.listView, scrollArea);
	}
	ui.listView->hide();
	ui.listView->deleteLater();

	connect(ui.actionButtonAdd, &QPushButton::clicked, this, &AutomationDialog::onAddRule);
	connect(ui.actionButtonRemove, &QPushButton::clicked, this, &AutomationDialog::onRemoveRule);

	rebuildRuleWidgets();
}

AutomationDialog::~AutomationDialog()
{}

void AutomationDialog::rebuildRuleWidgets()
{
	// Clear existing widgets
	while (QLayoutItem* item = m_rulesLayout->takeAt(0)) {
		if (item->widget()) item->widget()->deleteLater();
		delete item;
	}

	const auto& rules = m_rulesController.rules();
	for (int i = 0; i < static_cast<int>(rules.size()); ++i) {
		auto* w = new AutomationRuleItemWidget(m_elementsController, m_sessionController);
		w->setRule(rules[i]);
		m_rulesLayout->addWidget(w);

		connect(w, &AutomationRuleItemWidget::editingFinished, this, [this, i, w]() {
			saveWidgetToController(i, w);
		});

		connect(w, &AutomationRuleItemWidget::clicked, this, [this, i]() {
			selectRule(i);
		});
	}

	// Absorb any remaining vertical space below the last rule widget
	m_rulesLayout->addStretch(1);

	// Keep previous selection if still in range, otherwise select last
	if (!rules.empty()) {
		selectRule(qBound(0, m_selectedIndex, static_cast<int>(rules.size()) - 1));
	} else {
		m_selectedIndex = -1;
	}
}

void AutomationDialog::selectRule(int index)
{
	m_selectedIndex = index;

	// Walk all rule widgets and update their visual state
	for (int i = 0; i < m_rulesLayout->count(); ++i) {
		auto* item = m_rulesLayout->itemAt(i);
		if (!item || !item->widget()) continue;
		auto* w = qobject_cast<AutomationRuleItemWidget*>(item->widget());
		if (w) w->setSelected(i == m_selectedIndex);
	}
}

void AutomationDialog::saveWidgetToController(int row, AutomationRuleItemWidget* w)
{
	if (row < 0 || row >= static_cast<int>(m_rulesController.rules().size())) return;

	Rule r = w->rule();
	m_rulesController.updateRule(row, r);
}

void AutomationDialog::onAddRule()
{
	RuleTrigger trig(-1, AutomationEventStrings::PipelineStateChanged, QString());
	RuleAction act(-1, AutomationActionStrings::SessionStartRecording, QString());
	Rule r(-1, QStringLiteral("New rule"), true, trig, act);
	m_rulesController.addRule(r);

	// Select the newly added rule
	m_selectedIndex = static_cast<int>(m_rulesController.rules().size()) - 1;
	rebuildRuleWidgets();
}

void AutomationDialog::onRemoveRule()
{
	const int count = static_cast<int>(m_rulesController.rules().size());
	if (count == 0 || m_selectedIndex < 0 || m_selectedIndex >= count) return;

	m_rulesController.removeRule(m_selectedIndex);

	// Adjust selection: stay at same index or move up if we removed the last
	const int newCount = static_cast<int>(m_rulesController.rules().size());
	if (newCount == 0) {
		m_selectedIndex = -1;
	} else {
		m_selectedIndex = qMin(m_selectedIndex, newCount - 1);
	}

	rebuildRuleWidgets();
}

