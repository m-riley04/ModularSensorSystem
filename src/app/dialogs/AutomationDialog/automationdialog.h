#pragma once

#include <QDialog>
#include "ui_automationdialog.h"
#include <controllers/rulescontroller.hpp>

class AutomationDialog : public QDialog
{
	Q_OBJECT

private:
	Ui::AutomationDialogClass ui;
	RulesController& m_rulesController;

	void populateRuleList();

public:
	AutomationDialog(RulesController&, QWidget *parent = nullptr);
	~AutomationDialog();


};

