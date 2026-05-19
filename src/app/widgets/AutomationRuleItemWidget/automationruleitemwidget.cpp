#include "automationruleitemwidget.h"
#include <models/rule_models.hpp>
#include <controllers/rulescontroller.hpp>
#include <controllers/elementscontroller.hpp>
#include <controllers/sessioncontroller.hpp>
#include <features/element.hpp>
#include <widgets/widgets/GroupSelectWidget/groupselectwidget.h>
#include <qevent.h>

// TODO/CONSIDER: are definitions the right way to go about this? maybe use a style sheet instead?
#define AUTOMATION_RULE_ITEM_HEIGHT 60

AutomationRuleItemWidget::AutomationRuleItemWidget(RulesController& rc, ElementsController& ec, SessionController& sc, QWidget* parent)
	: QWidget(parent)
	, m_rulesController(rc)
	, m_elementsController(ec)
	, m_sessionController(sc)
{
	ui.setupUi(this);

	// The stacked widget is unused. Hide it so it doesn't affect layout
	ui.stackCondition->hide();

	// Prevent vertical stretching: lock to a fixed height
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	setMaximumHeight(AUTOMATION_RULE_ITEM_HEIGHT);

	// TODO: do we really need this?
	m_selectEventSources = ui.selectEventSources;
	m_selectEventTypes = ui.selectEventTypes;
	m_selectActionTargets = ui.selectActionTargets;
	m_selectActions      = ui.selectActions;

	populateTargets();

	if (m_selectActions)       m_selectActions->setPlaceholderText("Select action");
	if (m_selectActionTargets) m_selectActionTargets->setPlaceholderText("Select targets");
	if (m_selectEventSources)  m_selectEventSources->setPlaceholderText("Select event sources");
	if (m_selectEventTypes)    m_selectEventTypes->setPlaceholderText("Select event type");

	// When any selection changes, sync m_rule and notify
	for (auto* w : { m_selectActions, m_selectActionTargets, m_selectEventSources, m_selectEventTypes }) {
		if (!w) continue;
		connect(w, &GroupSelectWidget::selectionChanged, this, [this](const QList<GroupSelectWidget::Option>&) {
			if (m_updatingUi) return;
			syncRuleFromWidgets();
			emit editingFinished();
		});
	}

	connect(ui.checkboxToggleRule, &QCheckBox::toggled, this, [this](bool) {
		if (m_updatingUi) return;
		syncRuleFromWidgets();
		emit editingFinished();
	});

	setAutoFillBackground(true);
	updateSelectionStyle();
}

AutomationRuleItemWidget::~AutomationRuleItemWidget()
{}

void AutomationRuleItemWidget::setRule(const Rule& rule)
{
	m_updatingUi = true;
	m_rule = rule;

	ui.checkboxToggleRule->setChecked(m_rule.isActive());

	if (m_selectActions) {
		m_selectActions->setSelectionMode(GroupSelectWidget::SelectionMode::Single);
		m_selectActions->setSelectedUserData({ m_rule.action().actionType() });
	}
	if (m_selectActionTargets) {
		m_selectActionTargets->setSelectionMode(GroupSelectWidget::SelectionMode::Multi);
		const QString& raw = m_rule.action().target();
		QVariantList vals;
		if (raw.isEmpty()) {
			vals.append(QString("(Session)"));
		} else {
			for (const auto& s : raw.split(';', Qt::KeepEmptyParts)) {
				vals.append(s.isEmpty() ? QString("(Session)") : QVariant(s));
			}
		}
		m_selectActionTargets->setSelectedUserData(vals);
	}
	if (m_selectEventSources) {
		m_selectEventSources->setSelectionMode(GroupSelectWidget::SelectionMode::Multi);
		const QString& raw = m_rule.trigger().condition();
		QVariantList vals;
		if (raw.isEmpty()) {
			vals.append(QString("(Session)"));
		} else {
			for (const auto& s : raw.split(';', Qt::KeepEmptyParts)) {
				vals.append(s.isEmpty() ? QString("(Session)") : QVariant(s));
			}
		}
		m_selectEventSources->setSelectedUserData(vals);
	}
	if (m_selectEventTypes) {
		m_selectEventTypes->setSelectionMode(GroupSelectWidget::SelectionMode::Single);
		m_selectEventTypes->setSelectedUserData({ m_rule.trigger().eventType() });
	}

	m_updatingUi = false;
}

Rule AutomationRuleItemWidget::rule() const
{
	return m_rule;
}

void AutomationRuleItemWidget::setSelected(bool selected)
{
	if (m_selected == selected) return;
	m_selected = selected;
	updateSelectionStyle();
}

void AutomationRuleItemWidget::updateSelectionStyle()
{
	QPalette pal = palette();
	if (m_selected) {
		pal.setColor(QPalette::Window, palette().color(QPalette::Highlight).lighter(160));
	} else {
		pal.setColor(QPalette::Window, palette().color(QPalette::Base));
	}
	setPalette(pal);
}

void AutomationRuleItemWidget::mousePressEvent(QMouseEvent* event)
{
	emit clicked();
	QWidget::mousePressEvent(event);
}

void AutomationRuleItemWidget::syncRuleFromWidgets()
{
	m_rule.setActive(ui.checkboxToggleRule->isChecked());

	if (m_selectActions) {
		const auto& sel = m_selectActions->selectedValues();
		m_rule.action().setActionType(sel.isEmpty() ? QString() : sel.first().userData.toString());
	}
	if (m_selectActionTargets) {
		const auto& sel = m_selectActionTargets->selectedValues();
		QStringList vals;
		vals.reserve(sel.size());
		for (const auto& o : sel) {
			const QString v = o.userData.toString();
			vals.append(v == "(Session)" ? QString() : v);
		}
		m_rule.action().setTarget(vals.join(';'));
	}
	if (m_selectEventSources) {
		const auto& sel = m_selectEventSources->selectedValues();
		QStringList vals;
		vals.reserve(sel.size());
		for (const auto& o : sel) {
			const QString v = o.userData.toString();
			vals.append(v == "(Session)" ? QString() : v);
		}
		m_rule.trigger().setCondition(vals.join(';'));
	}
	if (m_selectEventTypes) {
		const auto& sel = m_selectEventTypes->selectedValues();
		m_rule.trigger().setEventType(sel.isEmpty() ? QString() : sel.first().userData.toString());
	}
}

void AutomationRuleItemWidget::populateTargets()
{
	if (m_selectActionTargets) {
		QSignalBlocker b(m_selectActionTargets);
		const auto& prev = m_selectActionTargets->selectedValues();
		m_selectActionTargets->clear();
		m_selectActionTargets->setSelectionMode(GroupSelectWidget::SelectionMode::Multi);
		m_selectActionTargets->addItem("(Session)", QString("(Session)"));
		for (auto* e : m_elementsController.elements()) {
			if (!e) continue;
			m_selectActionTargets->addItem(QString::fromStdString(e->displayName()), QString::fromStdString(e->id()));
		}
		m_selectActionTargets->setSelectedValues(prev);
	}

	if (m_selectEventSources) {
		QSignalBlocker b(m_selectEventSources);
		const auto& prev = m_selectEventSources->selectedValues();
		m_selectEventSources->clear();
		m_selectEventSources->setSelectionMode(GroupSelectWidget::SelectionMode::Multi);
		m_selectEventSources->addItem("(Session)", QString("(Session)"));
		for (auto* e : m_elementsController.elements()) {
			if (!e) continue;
			m_selectEventSources->addItem(QString::fromStdString(e->displayName()), QString::fromStdString(e->id()));
		}
		m_selectEventSources->setSelectedValues(prev);
	}

	// Event types — driven by RulesController registry
	if (m_selectEventTypes) {
		QSignalBlocker b(m_selectEventTypes);
		const auto& prev = m_selectEventTypes->selectedValues();
		m_selectEventTypes->clear();
		m_selectEventTypes->setSelectionMode(GroupSelectWidget::SelectionMode::Single);

		for (const auto& info : m_rulesController.registeredEventTypes()) {
			m_selectEventTypes->addItem(info.displayName, info.id);
		}
		m_selectEventTypes->setSelectedValues(prev);
	}

	// Action types — driven by RulesController registry
	if (m_selectActions) {
		QSignalBlocker b(m_selectActions);
		const auto& prev = m_selectActions->selectedValues();
		m_selectActions->clear();
		m_selectActions->setSelectionMode(GroupSelectWidget::SelectionMode::Single);

		for (const auto& info : m_rulesController.registeredActionTypes()) {
			m_selectActions->addItem(info.displayName, info.id);
		}
		m_selectActions->setSelectedValues(prev);
	}
}

QSize AutomationRuleItemWidget::sizeHint() const
{
	return QSize(689, 60);
}