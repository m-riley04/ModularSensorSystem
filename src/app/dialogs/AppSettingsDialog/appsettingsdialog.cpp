#include "appsettingsdialog.h"

AppSettingsDialog::AppSettingsDialog(SettingsController& sc, UiSettingsController& uic, QWidget *parent)
	: QDialog(parent), m_settingsController(sc), m_uiSettingsController(uic)
{
	ui.setupUi(this);

	// Set label for settings file path
	ui.labelSettingsFilePath->setText(m_settingsController.settings().fileName());

	loadSettingsIntoUi();

	connect(ui.buttonBox, &QDialogButtonBox::clicked, this, &AppSettingsDialog::onButtonBoxClicked);

	/// GENERAL TAB SETUP
	connect(ui.checkboxCloseToTray, &QCheckBox::toggled, &m_settingsController, &SettingsController::setCloseToTray);
	connect(ui.checkboxCheckForUpdates, &QCheckBox::toggled, &m_settingsController, &SettingsController::setCheckForUpdatesOnStartup);
	connect(ui.dropdownLanguage, &QComboBox::currentTextChanged, &m_settingsController, &SettingsController::setLanguage);

	/// ADVANCED TAB SETUP
	connect(ui.checkboxLogging, &QCheckBox::toggled, &m_settingsController, &SettingsController::setEnableLogging);
	connect(ui.checkboxDebug, &QCheckBox::toggled, &m_settingsController, &SettingsController::setEnableDebugMode);

	/// APPEARANCE TAB SETUP

	/// SESSION TAB SETUP
	connect(ui.checkboxConfirmStopSession, &QCheckBox::toggled, &m_settingsController, &SettingsController::setConfirmOnStopSession);
	connect(ui.checkboxConfirmStopRecording, &QCheckBox::toggled, &m_settingsController, &SettingsController::setConfirmOnStopRecording);
	connect(ui.dirPickerRecording, &QDirectoryPickerWidget::directoryChanged, &m_settingsController, &SettingsController::setOutputDirectory);
	connect(ui.lineRecordingPrefix, &QLineEdit::textChanged, &m_settingsController, &SettingsController::setOutputPrefix);
	connect(ui.checkboxOverwrite, &QCheckBox::toggled, &m_settingsController, &SettingsController::setOverwriteExistingFiles);
	connect(ui.checkboxAllowSpaces, &QCheckBox::toggled, &m_settingsController, &SettingsController::setAllowSpacesInFilenames);
	connect(ui.checkboxEnableClipping, &QCheckBox::toggled, &m_settingsController, &SettingsController::setEnableClipping);

	/// SOURCES TAB SETUP
	// video sources
	connect(ui.spinboxVideoWidth, &QSpinBox::valueChanged, [this](int width) {
		int height = ui.spinboxVideoHeight->value();
		m_settingsController.setDefaultVideoResolution(QSize(width, height));
		});
	connect(ui.spinboxVideoHeight, &QSpinBox::valueChanged, [this](int height) {
		int width = ui.spinboxVideoWidth->value();
		m_settingsController.setDefaultVideoResolution(QSize(width, height));
		});
	connect(ui.spinboxVideoFps, &QSpinBox::valueChanged , &m_settingsController, &SettingsController::setDefaultVideoFramerate); // TODO: make sure this works with double
	connect(ui.spinboxVideoBitrate, &QSpinBox::valueChanged, &m_settingsController, &SettingsController::setDefaultVideoBitrateKbps);
	connect(ui.dropdownPixelFormat, &QComboBox::currentTextChanged, &m_settingsController, &SettingsController::setDefaultPixelFormat);
	connect(ui.dropdownVideoContainerFormat, &QComboBox::currentTextChanged, &m_settingsController, &SettingsController::setDefaultVideoContainerFormat);
	connect(ui.dropdownVideoEncoder, &QComboBox::currentTextChanged, &m_settingsController, &SettingsController::setDefaultVideoCodec);
	// audio sources
	connect(ui.dropdownSampleRate, &QComboBox::currentTextChanged, [this](const QString& sampleRateStr) {
		bool ok;
		int sampleRate = sampleRateStr.toInt(&ok);
		if (ok) {
			m_settingsController.setDefaultAudioSampleRate(sampleRate);
		}
		});
	connect(ui.radioButtonMono, &QRadioButton::toggled, [this](bool checked) {
		bool stereoSelected = ui.radioButtonStereo->isChecked();
		m_settingsController.setDefaultAudioStereo(stereoSelected);
		});
	connect(ui.radioButtonStereo, &QRadioButton::toggled, [this](bool checked) {
		m_settingsController.setDefaultAudioStereo(checked);
		});
	connect(ui.dropdownAudioContainerFormat, &QComboBox::currentTextChanged, &m_settingsController, &SettingsController::setDefaultAudioContainerFormat);
	connect(ui.dropdownAudioEncoder, &QComboBox::currentTextChanged, &m_settingsController, &SettingsController::setDefaultAudioCodec);

	/// MOUNTS TAB SETUP
	// TODO

	/// PROCESSORS TAB SETUP
	// TODO

	/// PRESETS TAB SETUP
	connect(ui.dirPickerPresets, &QDirectoryPickerWidget::directoryChanged, &m_settingsController, &SettingsController::setPresetDirectory);

	/// PLUGINS TAB SETUP
	connect(ui.dirPickerPluginsRoot, &QDirectoryPickerWidget::directoryChanged, &m_settingsController, &SettingsController::setPluginsDirectory);

	/// KEYBINDS TAB SETUP
	connect(ui.keyToggleSession, &QKeySequenceEdit::keySequenceChanged, &m_uiSettingsController, &UiSettingsController::setToggleSessionKeybinding);
	connect(ui.keyToggleRecording, &QKeySequenceEdit::keySequenceChanged, &m_uiSettingsController, &UiSettingsController::setToggleRecordingKeybinding);
	connect(ui.keyClip, &QKeySequenceEdit::keySequenceChanged, &m_uiSettingsController, &UiSettingsController::setClipSessionKeybinding);

	/// ACCESSIBILITY TAB SETUP
	// TODO
}

AppSettingsDialog::~AppSettingsDialog()
{}

void AppSettingsDialog::onButtonBoxClicked(QAbstractButton* button)
{
	if (button == ui.buttonBox->button(QDialogButtonBox::Reset)) onResetClicked();
	else if (button == ui.buttonBox->button(QDialogButtonBox::RestoreDefaults)) onRestoreDefaultsClicked();
	else if (button == ui.buttonBox->button(QDialogButtonBox::Apply)) onApplyClicked();
	else reject();
}

void AppSettingsDialog::onResetClicked()
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, tr("Reset Settings"), tr("Are you sure you want to reset settings to their previous values?"), QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::No) return;

	// TODO: Reset settings to previous

	loadSettingsIntoUi();
}

void AppSettingsDialog::onRestoreDefaultsClicked()
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

void AppSettingsDialog::onApplyClicked()
{
	m_settingsController.saveSettings();
	m_uiSettingsController.saveSettings();
	accept();
}

void AppSettingsDialog::loadSettingsIntoUi()
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

	// Appearance tab
	AppearanceSettings appearanceSettings = m_uiSettingsController.appearanceSettings();

	// Session tab
	SessionSettings sessionSettings = m_settingsController.sessionSettings();
	ui.checkboxConfirmStopSession->setChecked(sessionSettings.confirmOnStopSession);
	ui.checkboxConfirmStopRecording->setChecked(sessionSettings.confirmOnStopRecording);
	ui.dirPickerRecording->setSelectedDirectory(sessionSettings.outputDirectory);
	ui.lineRecordingPrefix->setText(sessionSettings.outputPrefix);
	ui.checkboxOverwrite->setChecked(sessionSettings.overwriteExistingFiles);
	ui.checkboxAllowSpaces->setChecked(sessionSettings.allowSpacesInFilenames);
	ui.checkboxEnableClipping->setChecked(sessionSettings.enableClipping);

	// Sources tab
	SourcesSettings sourcesSettings = m_settingsController.sourcesSettings();
	// TODO: set resolution fields
	ui.spinboxVideoBitrate->setValue(sourcesSettings.defaultVideoBitrateKbps);
	ui.spinboxVideoFps->setValue(static_cast<int>(sourcesSettings.defaultVideoFramerate)); // TODO: check double to int conversion
	ui.dropdownPixelFormat->setCurrentText(sourcesSettings.defaultPixelFormat);
	ui.dropdownVideoContainerFormat->setCurrentText(sourcesSettings.defaultVideoContainerFormat);
	ui.dropdownVideoEncoder->setCurrentText(sourcesSettings.defaultVideoCodec);
	ui.dropdownSampleRate->setCurrentText(QString::number(sourcesSettings.defaultAudioSampleRate));
	// TODO: set stereo/mono radio buttons
	ui.dropdownAudioContainerFormat->setCurrentText(sourcesSettings.defaultAudioContainerFormat);
	ui.dropdownAudioEncoder->setCurrentText(sourcesSettings.defaultAudioCodec);
	ui.dropdownDataFormat->setCurrentText(sourcesSettings.defaultDataFormat);

	// Mounts tab
	MountSettings mountSettings = m_settingsController.mountSettings();

	// Processor tab
	ProcessorSettings processorSettings = m_settingsController.processorSettings();

	// Presets tab
	PresetSettings presetSettings = m_settingsController.presetSettings();
	ui.dirPickerPresets->setSelectedDirectory(presetSettings.presetDirectory);

	// Plugins tab
	PluginsSettings pluginsSettings = m_settingsController.pluginsSettings();
	ui.dirPickerPluginsRoot->setSelectedDirectory(pluginsSettings.pluginsDirectory);

	// Keybinds tab
	KeybindingSettings keybindingSettings = m_uiSettingsController.keybindingSettings();
	ui.keyToggleSession->setKeySequence(keybindingSettings.toggleSession);
	ui.keyToggleRecording->setKeySequence(keybindingSettings.toggleRecording);
	ui.keyClip->setKeySequence(keybindingSettings.clipSession);
	
	// Accessibility tab
	AccessibilitySettings accessibilitySettings = m_uiSettingsController.accessibilitySettings();

}