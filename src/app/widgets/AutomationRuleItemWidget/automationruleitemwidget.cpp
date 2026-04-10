#include "automationruleitemwidget.h"

#include <models/rule_models.hpp>

#include <controllers/elementscontroller.hpp>
#include <controllers/sessioncontroller.hpp>
#include <features/element.hpp>

#include <widgets/widgets/GroupSelectWidget/groupselectwidget.h>

#include <QMouseEvent>

static GroupSelectWidget* replacePlaceholderWithGroupSelect(Ui::AutomationRuleItemWidgetClass& ui, QWidget* parent, const char* objectName)
{
	auto* placeholder = parent->findChild<QWidget*>(objectName);
	if (!placeholder) return nullptr;
	if (auto* existing = dynamic_cast<GroupSelectWidget*>(placeholder)) return existing;

	const QString placeholderName = placeholder->objectName();

	auto* gs = new GroupSelectWidget(parent);
	gs->setObjectName(placeholderName);
	gs->setSizePolicy(placeholder->sizePolicy());
	gs->setMinimumSize(placeholder->minimumSize());
	gs->setMaximumSize(placeholder->maximumSize());

	if (placeholder == ui.selectEventSources) {
		ui.layoutEvent->replaceWidget(ui.selectEventSources, gs);
		ui.selectEventSources->hide();
		ui.selectEventSources = gs;
	}
	else if (placeholder == ui.selectEventTypes) {
		ui.layoutEvent->replaceWidget(ui.selectEventTypes, gs);
		ui.selectEventTypes->hide();
		ui.selectEventTypes = gs;
	}
	else if (placeholder == ui.selectActionTargets) {
		ui.layoutAction->replaceWidget(ui.selectActionTargets, gs);
		ui.selectActionTargets->hide();
		ui.selectActionTargets = gs;
	}
	else if (placeholder == ui.selectActions) {
		ui.layoutAction->replaceWidget(ui.selectActions, gs);
		ui.selectActions->hide();
		ui.selectActions = gs;
	}
	else {
		delete gs;
		return nullptr;
	}

	gs->show();
	placeholder->deleteLater();
	return gs;
}

AutomationRuleItemWidget::AutomationRuleItemWidget(ElementsController& ec, SessionController& sc, QWidget* parent)
	: QWidget(parent)
	, m_elementsController(ec)
	, m_sessionController(sc)
{
	ui.setupUi(this);

	// The stacked widget is unused — hide it so it doesn't affect layout
	ui.stackCondition->hide();

	// Prevent vertical stretching: lock to a fixed height
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	setMaximumHeight(60);

	// Replace .ui placeholders with real GroupSelectWidgets
	m_selectEventSources = replacePlaceholderWithGroupSelect(ui, this, "selectEventSources");
	m_selectEventTypes   = replacePlaceholderWithGroupSelect(ui, this, "selectEventTypes");
	m_selectActionTargets = replacePlaceholderWithGroupSelect(ui, this, "selectActionTargets");
	m_selectActions      = replacePlaceholderWithGroupSelect(ui, this, "selectActions");

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
		const auto sel = m_selectActions->selectedValues();
		m_rule.action().setActionType(sel.isEmpty() ? QString() : sel.first().userData.toString());
	}
	if (m_selectActionTargets) {
		const auto sel = m_selectActionTargets->selectedValues();
		QStringList vals;
		vals.reserve(sel.size());
		for (const auto& o : sel) {
			const QString v = o.userData.toString();
			vals.append(v == "(Session)" ? QString() : v);
		}
		m_rule.action().setTarget(vals.join(';'));
	}
	if (m_selectEventSources) {
		const auto sel = m_selectEventSources->selectedValues();
		QStringList vals;
		vals.reserve(sel.size());
		for (const auto& o : sel) {
			const QString v = o.userData.toString();
			vals.append(v == "(Session)" ? QString() : v);
		}
		m_rule.trigger().setCondition(vals.join(';'));
	}
	if (m_selectEventTypes) {
		const auto sel = m_selectEventTypes->selectedValues();
		m_rule.trigger().setEventType(sel.isEmpty() ? QString() : sel.first().userData.toString());
	}
}

void AutomationRuleItemWidget::populateTargets()
{
	if (m_selectActionTargets) {
		QSignalBlocker b(m_selectActionTargets);
		auto prev = m_selectActionTargets->selectedValues();
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
		auto prev = m_selectEventSources->selectedValues();
		m_selectEventSources->clear();
		m_selectEventSources->setSelectionMode(GroupSelectWidget::SelectionMode::Multi);
		m_selectEventSources->addItem("(Session)", QString("(Session)"));
		for (auto* e : m_elementsController.elements()) {
			if (!e) continue;
			m_selectEventSources->addItem(QString::fromStdString(e->displayName()), QString::fromStdString(e->id()));
		}
		m_selectEventSources->setSelectedValues(prev);
	}

	if (m_selectEventTypes) {
		QSignalBlocker b(m_selectEventTypes);
		auto prev = m_selectEventTypes->selectedValues();
		m_selectEventTypes->clear();
		m_selectEventTypes->setSelectionMode(GroupSelectWidget::SelectionMode::Single);
		m_selectEventTypes->addItem("Pipeline state changed", AutomationEventStrings::PipelineStateChanged);
		m_selectEventTypes->addItem("Pipeline EOS",            AutomationEventStrings::PipelineEos);
		m_selectEventTypes->addItem("Pipeline error",          AutomationEventStrings::PipelineError);
		m_selectEventTypes->addItem("Recording started",       AutomationEventStrings::RecordingStarted);
		m_selectEventTypes->addItem("Recording stopped",       AutomationEventStrings::RecordingStopped);
		m_selectEventTypes->setSelectedValues(prev);
	}

	if (m_selectActions) {
		QSignalBlocker b(m_selectActions);
		auto prev = m_selectActions->selectedValues();
		m_selectActions->clear();
		m_selectActions->setSelectionMode(GroupSelectWidget::SelectionMode::Single);
		m_selectActions->addItem("Start recording",        AutomationActionStrings::SessionStartRecording);
		m_selectActions->addItem("Stop recording",         AutomationActionStrings::SessionStopRecording);
		m_selectActions->addItem("Start processing",       AutomationActionStrings::SessionStartProcessing);
		m_selectActions->addItem("Stop processing",        AutomationActionStrings::SessionStopProcessing);
		m_selectActions->addItem("Move to\u2026",          QStringLiteral("mount.moveTo"));
		m_selectActions->addItem("Follow detected object", QStringLiteral("mount.followObject"));
		m_selectActions->setSelectedValues(prev);
	}
}

QSize AutomationRuleItemWidget::sizeHint() const
{
	return QSize(689, 60);
}