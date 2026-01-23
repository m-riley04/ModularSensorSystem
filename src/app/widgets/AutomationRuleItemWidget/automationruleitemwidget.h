#pragma once

#include <QWidget>
#include "ui_automationruleitemwidget.h"
#include <models/rule_models.hpp>

class ElementsController;
class SessionController;

// NOTE: structured after Qt docs for item delegate: https://doc.qt.io/qt-6/qtwidgets-itemviews-stardelegate-example.html

class AutomationRuleItemWidget : public QWidget
{
	Q_OBJECT

private:
	Ui::AutomationRuleItemWidgetClass ui;
	Rule m_rule;
	ElementsController& m_elementsController;
	SessionController& m_sessionController;

	void populateTargets();

protected:
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

public:
	AutomationRuleItemWidget(ElementsController& ec, SessionController& sc, QWidget *parent = nullptr);
	~AutomationRuleItemWidget();

	QSize sizeHint() const override;
	void setRule(const Rule& rule);
	Rule rule() const { return m_rule; }

signals:
	void editingFinished();

};

