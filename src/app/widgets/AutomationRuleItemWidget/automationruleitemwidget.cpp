#include "automationruleitemwidget.h"

#include <models/rule_models.hpp>

#include <controllers/elementscontroller.hpp>
#include <controllers/sessioncontroller.hpp>
#include <features/element.hpp>

#include <widgets/widgets/GroupSelectWidget/groupselectwidget.h>

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

// NOTE: structured after Qt docs for item delegate: https://doc.qt.io/qt-6/qtwidgets-itemviews-stardelegate-example.html

AutomationRuleItemWidget::AutomationRuleItemWidget(ElementsController& ec, SessionController& sc, QWidget *parent)
	: QWidget(parent)
	, m_elementsController(ec)
	, m_sessionController(sc)
{
	ui.setupUi(this);

	// The .ui contains generic QWidget placeholders. Replace them with real widgets at runtime.
	replacePlaceholderWithGroupSelect(ui, this, "selectEventSources");
	replacePlaceholderWithGroupSelect(ui, this, "selectEventTypes");
	replacePlaceholderWithGroupSelect(ui, this, "selectActionTargets");
	replacePlaceholderWithGroupSelect(ui, this, "selectActions");

	// The .ui contains native placeholders; discover GroupSelectWidget at runtime.
	populateTargets();

	for (auto* w : { dynamic_cast<GroupSelectWidget*>(ui.selectActions),
				  dynamic_cast<GroupSelectWidget*>(ui.selectActionTargets),
				  dynamic_cast<GroupSelectWidget*>(ui.selectEventSources),
				  dynamic_cast<GroupSelectWidget*>(ui.selectEventTypes) }) {
		if (!w) continue;
		connect(w, &GroupSelectWidget::selectionChanged, this, [this](const QList<GroupSelectWidget::Option>&) {
			if (m_updatingUi) return;
			emit editingFinished();
		});
	}

	if (auto* w = dynamic_cast<GroupSelectWidget*>(ui.selectActions)) {
		w->setPlaceholderText("Select action");
	}
	if (auto* w = dynamic_cast<GroupSelectWidget*>(ui.selectActionTargets)) {
		w->setPlaceholderText("Select targets");
	}
	if (auto* w = dynamic_cast<GroupSelectWidget*>(ui.selectEventSources)) {
		w->setPlaceholderText("Select event sources");
	}
	if (auto* w = dynamic_cast<GroupSelectWidget*>(ui.selectEventTypes)) {
		w->setPlaceholderText("Select event type");
	}

	setMouseTracking(true);
	setAutoFillBackground(true);

	// Persist when any selection changes
	connect(ui.checkboxToggleRule, &QCheckBox::toggled, this, [this](bool) {
		if (m_updatingUi) return;
		emit editingFinished();
	});
}

AutomationRuleItemWidget::~AutomationRuleItemWidget()
{}

void AutomationRuleItemWidget::mouseMoveEvent(QMouseEvent* event)
{
	// TODO
	QWidget::mouseMoveEvent(event);
}

void AutomationRuleItemWidget::mouseReleaseEvent(QMouseEvent* event)
{
	emit editingFinished();
	QWidget::mouseReleaseEvent(event);
}

void AutomationRuleItemWidget::setRule(const Rule& rule)
{
	m_updatingUi = true;
	m_rule = rule;
	ui.checkboxToggleRule->setChecked(m_rule.isActive());

	populateTargets();

	if (auto* actions = dynamic_cast<GroupSelectWidget*>(ui.selectActions)) {
		QSignalBlocker blocker(actions);
		actions->setSelectionMode(GroupSelectWidget::SelectionMode::Single);
		const QVariant ud = QString::fromStdString(toString(m_rule.action().actionType()));
		actions->setSelectedUserData({ ud });
	}
	if (auto* targets = dynamic_cast<GroupSelectWidget*>(ui.selectActionTargets)) {
		QSignalBlocker blocker(targets);
		targets->setSelectionMode(GroupSelectWidget::SelectionMode::Multi);
		{
			const QString raw = QString::fromStdString(m_rule.action().target());
			QVariantList vals;
			if (raw.isEmpty()) {
				vals.append(QString("(Session)"));
			}
			else {
				for (const auto& s : raw.split(';', Qt::KeepEmptyParts)) {
					if (s.isEmpty()) vals.append(QString("(Session)"));
					else vals.append(s);
				}
			}
			targets->setSelectedUserData(vals);
		}
	}
	if (auto* evSrc = dynamic_cast<GroupSelectWidget*>(ui.selectEventSources)) {
		QSignalBlocker blocker(evSrc);
		evSrc->setSelectionMode(GroupSelectWidget::SelectionMode::Multi);
		{
			const QString raw = QString::fromStdString(m_rule.trigger().condition());
			QVariantList vals;
			if (raw.isEmpty()) {
				vals.append(QString("(Session)"));
			}
			else {
				for (const auto& s : raw.split(';', Qt::KeepEmptyParts)) {
					if (s.isEmpty()) vals.append(QString("(Session)"));
					else vals.append(s);
				}
			}
			evSrc->setSelectedUserData(vals);
		}
	}
	if (auto* evTypes = dynamic_cast<GroupSelectWidget*>(ui.selectEventTypes)) {
		QSignalBlocker blocker(evTypes);
		evTypes->setSelectionMode(GroupSelectWidget::SelectionMode::Single);
		evTypes->setSelectedUserData({ QString::fromStdString(toString(m_rule.trigger().triggerType())) });
	}
	m_updatingUi = false;
}

void AutomationRuleItemWidget::populateTargets()
{
	QSignalBlocker b1(ui.selectActionTargets);
	QSignalBlocker b2(ui.selectEventSources);
	QSignalBlocker b3(ui.selectActions);
	QSignalBlocker b4(ui.selectEventTypes);

	auto* actionTargets = dynamic_cast<GroupSelectWidget*>(ui.selectActionTargets);
	auto* eventSources = dynamic_cast<GroupSelectWidget*>(ui.selectEventSources);
	auto* actions = dynamic_cast<GroupSelectWidget*>(ui.selectActions);
	auto* eventTypes = dynamic_cast<GroupSelectWidget*>(ui.selectEventTypes);

	const QList<GroupSelectWidget::Option> prevActionTargets = actionTargets ? actionTargets->selectedValues() : QList<GroupSelectWidget::Option>{};
	const QList<GroupSelectWidget::Option> prevEventSources = eventSources ? eventSources->selectedValues() : QList<GroupSelectWidget::Option>{};
	const QList<GroupSelectWidget::Option> prevActions = actions ? actions->selectedValues() : QList<GroupSelectWidget::Option>{};
	const QList<GroupSelectWidget::Option> prevEventTypes = eventTypes ? eventTypes->selectedValues() : QList<GroupSelectWidget::Option>{};

	if (actionTargets) {
		actionTargets->clear();
		actionTargets->addItem("(Session)", QString("(Session)"));
		for (auto* e : m_elementsController.elements()) {
			if (!e) continue;
			actionTargets->addItem(QString::fromStdString(e->displayName()), QString::fromStdString(e->id()));
		}
	}

	if (eventSources) {
		eventSources->clear();
		eventSources->addItem("(Session)", QString("(Session)"));
		for (auto* e : m_elementsController.elements()) {
			if (!e) continue;
			eventSources->addItem(QString::fromStdString(e->displayName()), QString::fromStdString(e->id()));
		}
	}

	if (eventTypes) {
		eventTypes->clear();
		eventTypes->setSelectionMode(GroupSelectWidget::SelectionMode::Single);
		eventTypes->addItem("Automation event", QString::fromStdString(toString(RuleTriggerType::AutomationEventType)));
	}

	if (actions) {
		actions->clear();
		actions->setSelectionMode(GroupSelectWidget::SelectionMode::Single);
		actions->addItem("Start recording", QString::fromStdString(toString(RuleActionType::SessionStartRecording)));
		actions->addItem("Stop recording", QString::fromStdString(toString(RuleActionType::SessionStopRecording)));
		actions->addItem("Start processing", QString::fromStdString(toString(RuleActionType::SessionStartProcessing)));
		actions->addItem("Stop processing", QString::fromStdString(toString(RuleActionType::SessionStopProcessing)));
		actions->addItem("Move to…", "mount.moveTo");
		actions->addItem("Follow detected object", "mount.followObject");
	}

	if (actionTargets) actionTargets->setSelectedValues(prevActionTargets);
	if (eventSources) eventSources->setSelectedValues(prevEventSources);
	if (actions) actions->setSelectedValues(prevActions);
	if (eventTypes) eventTypes->setSelectedValues(prevEventTypes);
}


// (no extra forward declaration)

QSize AutomationRuleItemWidget::sizeHint() const
{
	return QSize(200, 48);
}