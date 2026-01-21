#pragma once

#include <QWidget>
#include "ui_automationruleitemwidget.h"
#include <automation/automation.hpp>

// NOTE: structured after Qt docs for item delegate: https://doc.qt.io/qt-6/qtwidgets-itemviews-stardelegate-example.html

class AutomationRuleItemWidget : public QWidget
{
	Q_OBJECT

private:
	Ui::AutomationRuleItemWidgetClass ui;
	Rule& m_rule;

protected:
	void paintEvent(QPaintEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

public:
	AutomationRuleItemWidget(Rule& rule, QWidget *parent = nullptr);
	~AutomationRuleItemWidget();

	QSize sizeHint() const override;
	void setRule(const Rule& rule) {
		m_rule = rule;
	}
	Rule rule() const { return m_rule; }

signals:
	void editingFinished();

};

