#pragma once

#include <QDialog>
#include <QSettings>
#include "ui_apppropertiesdialog.h"
#include <controllers/settingscontroller.hpp>

// Not an enum class to allow implicit conversion to int for stacked widget index
enum AppPropertiesTab {
	General,
	Appearance,
	Session,
	Sources,
	Keybinds,
	Accessibility,
	Advanced
};

class AppPropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	AppPropertiesDialog(SettingsController& sc, QWidget *parent = nullptr);
	~AppPropertiesDialog();

private slots:
	void onGeneralTabClicked() { ui.stackedWidget->setCurrentIndex(AppPropertiesTab::General); }
	void onAppearanceTabClicked() { ui.stackedWidget->setCurrentIndex(AppPropertiesTab::Appearance); }
	void onSessionTabClicked() { ui.stackedWidget->setCurrentIndex(AppPropertiesTab::Session); }
	void onSourcesTabClicked() { ui.stackedWidget->setCurrentIndex(AppPropertiesTab::Sources); }
	void onKeybindsTabClicked() { ui.stackedWidget->setCurrentIndex(AppPropertiesTab::Keybinds); }
	void onAccessibilityTabClicked() { ui.stackedWidget->setCurrentIndex(AppPropertiesTab::Accessibility); }
	void onAdvancedTabClicked() { ui.stackedWidget->setCurrentIndex(AppPropertiesTab::Advanced); }

	void onButtonBoxClicked(QAbstractButton* button);
	void onResetClicked();
	void onRestoreDefaultsClicked();
	void onApplyClicked();

private:
	Ui::AppPropertiesDialogClass ui;
	SettingsController& m_settingsController;
};

