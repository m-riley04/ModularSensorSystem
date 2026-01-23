#pragma once

#include <QDialog>
#include "ui_automationdialog.h"
#include <controllers/rulescontroller.hpp>

class ElementsController;
class SessionController;

class AutomationRulesListModel;

class AutomationDialog : public QDialog
{
	Q_OBJECT

private:
	Ui::AutomationDialogClass ui;
	RulesController& m_rulesController;
	ElementsController& m_elementsController;
	SessionController& m_sessionController;
	AutomationRulesListModel* m_rulesModel = nullptr;

	void populateRuleList();

private slots:
	void onAddRule();
	void onRemoveRule();

public:
	AutomationDialog(RulesController&, ElementsController&, SessionController&, QWidget *parent = nullptr);
	~AutomationDialog();


};

