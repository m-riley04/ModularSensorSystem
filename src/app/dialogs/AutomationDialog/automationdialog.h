#pragma once

#include <QDialog>
#include "ui_automationdialog.h"

class AutomationDialog : public QDialog
{
	Q_OBJECT

public:
	AutomationDialog(QWidget *parent = nullptr);
	~AutomationDialog();

private:
	Ui::AutomationDialogClass ui;
};

