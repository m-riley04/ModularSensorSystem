#include "automationruleitemwidget.h"
#include <QPainter>

// NOTE: structured after Qt docs for item delegate: https://doc.qt.io/qt-6/qtwidgets-itemviews-stardelegate-example.html

AutomationRuleItemWidget::AutomationRuleItemWidget(Rule& rule, QWidget *parent)
	: QWidget(parent), m_rule(rule)
{
	ui.setupUi(this);

	setMouseTracking(true);
	setAutoFillBackground(true);
}

AutomationRuleItemWidget::~AutomationRuleItemWidget()
{}

void AutomationRuleItemWidget::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	m_rule.paint(&painter, rect(), palette(), Rule::EditMode::Editable);
}

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