#include "automationruleitemwidget.h"

#include <models/rule_models.hpp>

// NOTE: structured after Qt docs for item delegate: https://doc.qt.io/qt-6/qtwidgets-itemviews-stardelegate-example.html

AutomationRuleItemWidget::AutomationRuleItemWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.dropdownAction->clear();
	ui.dropdownAction->addItem("Start recording", QString::fromStdString(toString(RuleActionType::SessionStartRecording)));
	ui.dropdownAction->addItem("Stop recording", QString::fromStdString(toString(RuleActionType::SessionStopRecording)));
	ui.dropdownAction->addItem("Start processing", QString::fromStdString(toString(RuleActionType::SessionStartProcessing)));
	ui.dropdownAction->addItem("Stop processing", QString::fromStdString(toString(RuleActionType::SessionStopProcessing)));

	setMouseTracking(true);
	setAutoFillBackground(true);

	connect(ui.dropdownConditionTarget, &QComboBox::currentIndexChanged, this, &AutomationRuleItemWidget::editingFinished);
	connect(ui.dropdownType, &QComboBox::currentIndexChanged, this, &AutomationRuleItemWidget::editingFinished);
	connect(ui.dropdownActionTarget, &QComboBox::currentIndexChanged, this, &AutomationRuleItemWidget::editingFinished);
	connect(ui.dropdownAction, &QComboBox::currentIndexChanged, this, &AutomationRuleItemWidget::editingFinished);
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

void AutomationRuleItemWidget::setRule(const RuleModel& rule)
{
	m_rule = rule;

	const int idx = ui.dropdownAction->findData(QString::fromStdString(m_rule.action.actionType));
	if (idx >= 0) ui.dropdownAction->setCurrentIndex(idx);
}

QSize AutomationRuleItemWidget::sizeHint() const
{
	return QSize(200, 48);
}