#pragma once

#include <QWidget>
#include "ui_automationruleitemwidget.h"
#include <models/rule_models.hpp>

// NOTE: structured after Qt docs for item delegate: https://doc.qt.io/qt-6/qtwidgets-itemviews-stardelegate-example.html

class AutomationRuleItemWidget : public QWidget
{
	Q_OBJECT

private:
	Ui::AutomationRuleItemWidgetClass ui;
	RuleModel m_rule;

protected:
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

public:
	AutomationRuleItemWidget(QWidget *parent = nullptr);
	~AutomationRuleItemWidget();

	QSize sizeHint() const override;
	void setRule(const RuleModel& rule);
	RuleModel rule() const { return m_rule; }

signals:
	void editingFinished();

};

