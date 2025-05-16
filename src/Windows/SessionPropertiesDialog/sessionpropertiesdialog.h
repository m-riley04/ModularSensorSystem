#pragma once

#include <QDialog>
#include "ui_sessionpropertiesdialog.h"
#include <Controllers/MainController/maincontroller.h>

class SessionPropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	SessionPropertiesDialog(MainController* controller, QWidget *parent = nullptr);
	~SessionPropertiesDialog();

private:
	Ui::SessionPropertiesDialogClass ui;

	MainController* pController = nullptr;

	void initSignals();

public slots:
	void transmit(QAbstractButton* button);

};
