#include "apppropertiesdialog.h"

AppPropertiesDialog::AppPropertiesDialog(SettingsController& sc, UiSettingsController& uic, QWidget *parent)
	: QDialog(parent), m_settingsController(sc), m_uiSettingsController(uic)
{
	ui.setupUi(this);

	loadSettingsIntoUi();

	// Connect tab buttons to their respective slots
	connect(ui.buttonGeneral, &QPushButton::clicked, this, &AppPropertiesDialog::onGeneralTabClicked);
	connect(ui.buttonAdvanced, &QPushButton::clicked, this, &AppPropertiesDialog::onAdvancedTabClicked);
	connect(ui.buttonAppearance, &QPushButton::clicked, this, &AppPropertiesDialog::onAppearanceTabClicked);
	connect(ui.buttonSession, &QPushButton::clicked, this, &AppPropertiesDialog::onSessionTabClicked);
	connect(ui.buttonPlugins, &QPushButton::clicked, this, &AppPropertiesDialog::onPluginsTabClicked);
	connect(ui.buttonKeybinds, &QPushButton::clicked, this, &AppPropertiesDialog::onKeybindsTabClicked);
	connect(ui.buttonAccessibility, &QPushButton::clicked, this, &AppPropertiesDialog::onAccessibilityTabClicked);
	onGeneralTabClicked(); // Default to general tab

	connect(ui.buttonBox, &QDialogButtonBox::clicked, this, &AppPropertiesDialog::onButtonBoxClicked);

	/// GENERAL TAB SETUP
	connect(ui.checkboxCloseToTray, &QCheckBox::toggled, &m_settingsController, &SettingsController::setCloseToTray);
	connect(ui.checkboxCheckForUpdates, &QCheckBox::toggled, &m_settingsController, &SettingsController::setCheckForUpdatesOnStartup);
	connect(ui.dropdownLanguage, &QComboBox::currentTextChanged, &m_settingsController, &SettingsController::setLanguage);

	/// ADVANCED TAB SETUP
	connect(ui.checkboxLogging, &QCheckBox::toggled, &m_settingsController, &SettingsController::setEnableLogging);
	connect(ui.checkboxDebug, &QCheckBox::toggled, &m_settingsController, &SettingsController::setEnableDebugMode);

	/// SESSION TAB SETUP
	connect(ui.dirPickerRecording, &QDirectoryPickerWidget::directoryChanged, &m_settingsController, &SettingsController::setOutputDirectory);
	connect(ui.lineRecordingPrefix, &QLineEdit::textChanged, &m_settingsController, &SettingsController::setOutputPrefix);
	connect(ui.checkboxEnableClipping, &QCheckBox::toggled, &m_settingsController, &SettingsController::setEnableClipping);
}

AppPropertiesDialog::~AppPropertiesDialog()
{}

void AppPropertiesDialog::onButtonBoxClicked(QAbstractButton* button)
{
	if (button == ui.buttonBox->button(QDialogButtonBox::Reset)) onResetClicked();
	else if (button == ui.buttonBox->button(QDialogButtonBox::RestoreDefaults)) onRestoreDefaultsClicked();
	else if (button == ui.buttonBox->button(QDialogButtonBox::Apply)) onApplyClicked();
	else reject();
}

void AppPropertiesDialog::onResetClicked()
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, tr("Reset Settings"), tr("Are you sure you want to reset settings to their previous values?"), QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::No) return;

	// TODO: Reset settings to previous

	loadSettingsIntoUi();
}

void AppPropertiesDialog::onRestoreDefaultsClicked()
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, tr("Restore Default Settings"), tr("Are you sure you want to reset settings to their default values?"), QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::No) return;

	m_settingsController.restoreDefaultSettings();
	m_settingsController.saveSettings();
	m_uiSettingsController.restoreDefaultSettings();
	m_uiSettingsController.saveSettings();
	loadSettingsIntoUi();
}

void AppPropertiesDialog::onApplyClicked()
{
	m_settingsController.saveSettings();
	m_uiSettingsController.saveSettings();
	accept();
}

void AppPropertiesDialog::loadSettingsIntoUi()
{
	// General tab
	GeneralSettings generalSettings = m_settingsController.generalSettings();
	ui.checkboxCloseToTray->setChecked(generalSettings.closeToTray);
	ui.checkboxCheckForUpdates->setChecked(generalSettings.checkForUpdatesOnStartup);
	ui.dropdownLanguage->setCurrentText(generalSettings.language);
	// Advanced tab
	AdvancedSettings advancedSettings = m_settingsController.advancedSettings();
	ui.checkboxLogging->setChecked(advancedSettings.enableLogging);
	ui.checkboxDebug->setChecked(advancedSettings.enableDebugMode);
	// Session tab
	SessionSettings sessionSettings = m_settingsController.sessionSettings();
	ui.dirPickerRecording->setSelectedDirectory(sessionSettings.outputDirectory);
	ui.lineRecordingPrefix->setText(sessionSettings.outputPrefix);
	ui.checkboxEnableClipping->setChecked(sessionSettings.enableClipping);
	// Plugins tab
	PluginsSettings pluginsSettings = m_settingsController.pluginsSettings();
	ui.dirPickerPluginsRoot->setSelectedDirectory(pluginsSettings.pluginsDirectory);
}

void AppPropertiesDialog::resetButtonStates()
{
	ui.buttonGeneral->setChecked(false);
	ui.buttonAdvanced->setChecked(false);
	ui.buttonAppearance->setChecked(false);
	ui.buttonPlugins->setChecked(false);
	ui.buttonSession->setChecked(false);
	ui.buttonKeybinds->setChecked(false);
	ui.buttonAccessibility->setChecked(false);
}
