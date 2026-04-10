#pragma once

#include <QWidget>
#include "ui_automationruleitemwidget.h"
#include <models/rule_models.hpp>

class ElementsController;
class SessionController;
class GroupSelectWidget;

class AutomationRuleItemWidget : public QWidget
{
	Q_OBJECT

public:
	AutomationRuleItemWidget(ElementsController& ec, SessionController& sc, QWidget* parent = nullptr);
	~AutomationRuleItemWidget();

	QSize sizeHint() const override;
	void setRule(const Rule& rule);
	Rule rule() const;

	void setSelected(bool selected);
	bool isSelected() const { return m_selected; }

signals:
	void editingFinished();
	void clicked();

protected:
	void mousePressEvent(QMouseEvent* event) override;

private:
	void populateTargets();
	void syncRuleFromWidgets();
	void updateSelectionStyle();

	Ui::AutomationRuleItemWidgetClass ui;
	Rule m_rule;
	ElementsController& m_elementsController;
	SessionController& m_sessionController;
	bool m_updatingUi = false;
	bool m_selected = false;

	GroupSelectWidget* m_selectActions = nullptr;
	GroupSelectWidget* m_selectActionTargets = nullptr;
	GroupSelectWidget* m_selectEventSources = nullptr;
	GroupSelectWidget* m_selectEventTypes = nullptr;
};

