#include "automationruleitemwidget.h"

#include <models/rule_models.hpp>

#include <controllers/elementscontroller.hpp>
#include <controllers/sessioncontroller.hpp>
#include <features/element.hpp>

// NOTE: structured after Qt docs for item delegate: https://doc.qt.io/qt-6/qtwidgets-itemviews-stardelegate-example.html

AutomationRuleItemWidget::AutomationRuleItemWidget(ElementsController& ec, SessionController& sc, QWidget *parent)
	: QWidget(parent)
	, m_elementsController(ec)
	, m_sessionController(sc)
{
	ui.setupUi(this);

	ui.dropdownAction->clear();
	ui.dropdownAction->addItem("Start recording", QString::fromStdString(toString(RuleActionType::SessionStartRecording)));
	ui.dropdownAction->addItem("Stop recording", QString::fromStdString(toString(RuleActionType::SessionStopRecording)));
	ui.dropdownAction->addItem("Start processing", QString::fromStdString(toString(RuleActionType::SessionStartProcessing)));
	ui.dropdownAction->addItem("Stop processing", QString::fromStdString(toString(RuleActionType::SessionStopProcessing)));

	populateTargets();

	setMouseTracking(true);
	setAutoFillBackground(true);

	connect(ui.dropdownConditionTarget, &QComboBox::currentIndexChanged, this, &AutomationRuleItemWidget::editingFinished);
	connect(ui.dropdownType, &QComboBox::currentIndexChanged, this, &AutomationRuleItemWidget::editingFinished);
	connect(ui.dropdownActionTarget, &QComboBox::currentIndexChanged, this, &AutomationRuleItemWidget::editingFinished);
	connect(ui.dropdownAction, &QComboBox::currentIndexChanged, this, &AutomationRuleItemWidget::editingFinished);
	connect(ui.dropdownAction, &QComboBox::currentIndexChanged, this, [this]() { populateTargets(); });
	connect(ui.dropdownActionTarget, &QComboBox::currentIndexChanged, this, [this]() { populateTargets(); });
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
	m_rule = rule;

	const int idx = ui.dropdownAction->findData(QString::fromStdString(toString(m_rule.action().actionType())));
	if (idx >= 0) ui.dropdownAction->setCurrentIndex(idx);

	populateTargets();

	if (ui.dropdownActionTarget) {
		const int tIdx = ui.dropdownActionTarget->findData(QString::fromStdString(m_rule.action().target()));
		if (tIdx >= 0) ui.dropdownActionTarget->setCurrentIndex(tIdx);
	}
	if (ui.dropdownConditionTarget) {
		const int cIdx = ui.dropdownConditionTarget->findData(QString::fromStdString(m_rule.trigger().condition()));
		if (cIdx >= 0) ui.dropdownConditionTarget->setCurrentIndex(cIdx);
	}
}

static void addSessionActions(QComboBox* cb)
{
	cb->addItem("Start recording (session)", QString::fromStdString(toString(RuleActionType::SessionStartRecording)));
	cb->addItem("Stop recording (session)", QString::fromStdString(toString(RuleActionType::SessionStopRecording)));
	cb->addItem("Start processing (session)", QString::fromStdString(toString(RuleActionType::SessionStartProcessing)));
	cb->addItem("Stop processing (session)", QString::fromStdString(toString(RuleActionType::SessionStopProcessing)));
}

static void addElementActions(QComboBox* cb, Element* e)
{
	if (!e) return;

	if (e->asRecordable()) {
		cb->addItem("Start recording", QString::fromStdString(toString(RuleActionType::SessionStartRecording)));
		cb->addItem("Stop recording", QString::fromStdString(toString(RuleActionType::SessionStopRecording)));
	}
	if (e->asPipelineElement()) {
		cb->addItem("Start processing", QString::fromStdString(toString(RuleActionType::SessionStartProcessing)));
		cb->addItem("Stop processing", QString::fromStdString(toString(RuleActionType::SessionStopProcessing)));
	}
	if (e->elementType() == IElement::Type::Mount) {
		cb->addItem("Move to…", QString("mount.moveTo"));
		cb->addItem("Follow detected object", QString("mount.followObject"));
	}
}

static Element* findElementById(ElementsController& ec, const QString& id)
{
	for (auto* e : ec.elements()) {
		if (!e) continue;
		if (QString::fromStdString(e->id()) == id) return e;
	}
	return nullptr;
}

void AutomationRuleItemWidget::populateTargets()
{
	auto* cbActionTarget = ui.dropdownActionTarget;
	auto* cbCondTarget = ui.dropdownConditionTarget;
	auto* cbAction = ui.dropdownAction;
	if (!cbActionTarget || !cbCondTarget) return;

	const QString prevActionTarget = cbActionTarget->currentData().toString();
	const QString prevCondTarget = cbCondTarget->currentData().toString();

	cbActionTarget->blockSignals(true);
	cbCondTarget->blockSignals(true);
	cbActionTarget->clear();
	cbCondTarget->clear();

	// Allow de-select (session-wide)
	cbActionTarget->addItem("(Session)", QString());
	cbCondTarget->addItem("(Session)", QString());

	const auto elements = m_elementsController.elements();
	for (auto* e : elements) {
		if (!e) continue;

		const QString label = QString::fromStdString(e->displayName());
		const QString id = QString::fromStdString(e->id());
		cbActionTarget->addItem(label, id);

		// For now, condition target list is unfiltered.
		cbCondTarget->addItem(label, id);
	}

	const int idxAction = cbActionTarget->findData(prevActionTarget);
	if (idxAction >= 0) cbActionTarget->setCurrentIndex(idxAction);
	const int idxCond = cbCondTarget->findData(prevCondTarget);
	if (idxCond >= 0) cbCondTarget->setCurrentIndex(idxCond);

	cbActionTarget->blockSignals(false);
	cbCondTarget->blockSignals(false);

	// Rebuild actions list based on selected action target's capabilities
	if (cbAction) {
		const QString prevAction = cbAction->currentData().toString();
		cbAction->blockSignals(true);
		cbAction->clear();
		const QString selectedTargetId = cbActionTarget->currentData().toString();
		Element* target = findElementById(m_elementsController, selectedTargetId);
		if (target) {
			addElementActions(cbAction, target);
		}
		else {
			addSessionActions(cbAction);
		}
		int idx = cbAction->findData(prevAction);
		if (idx < 0 && cbAction->count() > 0) idx = 0;
		if (idx >= 0) cbAction->setCurrentIndex(idx);
		cbAction->blockSignals(false);
	}
}


// (no extra forward declaration)

QSize AutomationRuleItemWidget::sizeHint() const
{
	return QSize(200, 48);
}