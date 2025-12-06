#pragma once

#include <QDialog>
#include <QAbstractButton>
#include <QPushButton>
#include <qmessagebox.h>
#include "ui_appsettingsdialog.h"
#include <controllers/settingscontroller.hpp>
#include <controllers/UiSettingsController/uisettingscontroller.h>

// Forward declaration
class AppActionController;

class AppSettingsDialog : public QDialog
{
	Q_OBJECT

public:
	AppSettingsDialog(AppActionController& ac, SettingsController& sc, UiSettingsController& uic, QWidget *parent = nullptr);
	~AppSettingsDialog();

public slots:
	void onButtonBoxClicked(QAbstractButton* button);
	void onResetClicked();
	void onRestoreDefaultsClicked();
	void onApplyClicked();

private:
	Ui::AppSettingsDialogClass ui;
	AppActionController& m_actionController;
	SettingsController& m_settingsController;
	UiSettingsController& m_uiSettingsController;

	void loadSettingsIntoUi();

};

