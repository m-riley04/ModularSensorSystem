#pragma once

#include <QDialog>
#include "ui_applicationsettingsdialog.h"

class ApplicationSettingsDialog : public QDialog
{
	Q_OBJECT

public:
	ApplicationSettingsDialog(QWidget *parent = nullptr);
	~ApplicationSettingsDialog();

private:
	Ui::ApplicationSettingsDialogClass ui;
};
