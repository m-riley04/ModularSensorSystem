#pragma once

#include <QDialog>
#include <QSettings>
#include "ui_apppropertiesdialog.h"
#include <controllers/settingscontroller.hpp>
#include <controllers/UiSettingsController/uisettingscontroller.h>

// Not an enum class to allow implicit conversion to int for stacked widget index
enum AppPropertiesTab {
	General,
	Advanced,
	Appearance,
	Session,
	Plugins,
	Keybinds,
	Accessibility
};

class AppPropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	AppPropertiesDialog(SettingsController& sc, UiSettingsController& uic, QWidget *parent = nullptr);
	~AppPropertiesDialog();

private slots:
	void onGeneralTabClicked() { resetButtonStates(); ui.buttonGeneral->setChecked(true); ui.stackedWidget->setCurrentIndex(AppPropertiesTab::General); }
	void onAppearanceTabClicked() { resetButtonStates(); ui.buttonAppearance->setChecked(true); ui.stackedWidget->setCurrentIndex(AppPropertiesTab::Appearance); }
	void onSessionTabClicked() { resetButtonStates(); ui.buttonSession->setChecked(true); ui.stackedWidget->setCurrentIndex(AppPropertiesTab::Session); }
	void onPluginsTabClicked() { resetButtonStates(); ui.buttonPlugins->setChecked(true); ui.stackedWidget->setCurrentIndex(AppPropertiesTab::Plugins); }
	void onKeybindsTabClicked() { resetButtonStates(); ui.buttonKeybinds->setChecked(true); ui.stackedWidget->setCurrentIndex(AppPropertiesTab::Keybinds); }
	void onAccessibilityTabClicked() { resetButtonStates(); ui.buttonAccessibility->setChecked(true); ui.stackedWidget->setCurrentIndex(AppPropertiesTab::Accessibility); }
	void onAdvancedTabClicked() { resetButtonStates(); ui.buttonAdvanced->setChecked(true); ui.stackedWidget->setCurrentIndex(AppPropertiesTab::Advanced); }

	void onButtonBoxClicked(QAbstractButton* button);
	void onResetClicked();
	void onRestoreDefaultsClicked();
	void onApplyClicked();

private:
	Ui::AppPropertiesDialogClass ui;
	SettingsController& m_settingsController;
	UiSettingsController& m_uiSettingsController;

	void loadSettingsIntoUi();
	void resetButtonStates();
};

