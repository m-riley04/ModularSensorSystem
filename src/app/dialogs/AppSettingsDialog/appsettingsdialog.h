#pragma once

#include <QDialog>
#include "ui_appsettingsdialog.h"

class AppSettingsDialog : public QDialog
{
	Q_OBJECT

public:
	AppSettingsDialog(QWidget *parent = nullptr);
	~AppSettingsDialog();

private:
	Ui::AppSettingsDialogClass ui;
};

