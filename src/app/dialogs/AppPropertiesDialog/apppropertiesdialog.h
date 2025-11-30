#pragma once

#include <QDialog>
#include "ui_apppropertiesdialog.h"

class AppPropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	AppPropertiesDialog(QWidget *parent = nullptr);
	~AppPropertiesDialog();

private:
	Ui::AppPropertiesDialogClass ui;
};

