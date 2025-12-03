#pragma once

#include <QDialog>
#include <QAbstractButton>
#include <QPushButton>
#include <qmessagebox.h>
#include "ui_appsettingsdialog.h"
#include <controllers/settingscontroller.hpp>
#include <controllers/UiSettingsController/uisettingscontroller.h>

class AppSettingsDialog : public QDialog
{
	Q_OBJECT

public:
	AppSettingsDialog(SettingsController& sc, UiSettingsController& uic, QWidget *parent = nullptr);
	~AppSettingsDialog();

public slots:
	void onButtonBoxClicked(QAbstractButton* button);
	void onResetClicked();
	void onRestoreDefaultsClicked();
	void onApplyClicked();

private:
	Ui::AppSettingsDialogClass ui;
	SettingsController& m_settingsController;
	UiSettingsController& m_uiSettingsController;

	void loadSettingsIntoUi();

};

