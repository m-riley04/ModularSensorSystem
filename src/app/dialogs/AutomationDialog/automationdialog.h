#pragma once

#include "ui_automationdialog.h"
#include <controllers/rulescontroller.hpp>

class ElementsController;
class SessionController;
class AutomationRuleItemWidget;

class AutomationDialog : public QDialog
{
	Q_OBJECT

public:
	AutomationDialog(RulesController&, ElementsController&, SessionController&, QWidget* parent = nullptr);
	~AutomationDialog();

private slots:
	void onAddRule();
	void onRemoveRule();

private:
	Ui::AutomationDialogClass ui;
	RulesController& m_rulesController;
	ElementsController& m_elementsController;
	SessionController& m_sessionController;

	QVBoxLayout* m_rulesLayout = nullptr;
	int m_selectedIndex = -1;

	void rebuildRuleWidgets();
	void saveWidgetToController(int row, AutomationRuleItemWidget* w);
	void selectRule(int index);
};

