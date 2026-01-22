#pragma once

#include <QDialog>
#include "ui_automationdialog.h"
#include <controllers/rulescontroller.hpp>

class AutomationRulesListModel;

class AutomationDialog : public QDialog
{
	Q_OBJECT

private:
	Ui::AutomationDialogClass ui;
	RulesController& m_rulesController;
	AutomationRulesListModel* m_rulesModel = nullptr;

	void populateRuleList();

private slots:
	void onAddRule();
	void onRemoveRule();

public:
	AutomationDialog(RulesController&, QWidget *parent = nullptr);
	~AutomationDialog();


};

