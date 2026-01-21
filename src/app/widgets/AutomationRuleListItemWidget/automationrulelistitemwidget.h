#pragma once

#include <QWidget>
#include "ui_automationrulelistitemwidget.h"

class AutomationRuleListItemWidget : public QWidget
{
	Q_OBJECT

public:
	AutomationRuleListItemWidget(QWidget *parent = nullptr);
	~AutomationRuleListItemWidget();

private:
	Ui::AutomationRuleListItemWidgetClass ui;
};

