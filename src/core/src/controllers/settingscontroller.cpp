#include "controllers/settingscontroller.hpp"

SettingsController::SettingsController(QSettings& settings, QObject* parent)
	: QObject(parent), m_settings(settings)
{
	// Initialize default settings
	m_defaultGeneralSettings = GeneralSettings();
	m_defaultAdvancedSettings = AdvancedSettings();
	m_defaultSessionSettings = SessionSettings();
}

SettingsController::~SettingsController()
{
}

void SettingsController::loadSettings()
{
	m_settings.beginGroup(MSS_SETTINGS_GROUP_BACKEND); // Core settings group

	// General settings
	m_generalSettings.closeToTray = m_settings.value("general/closeToTray", m_defaultGeneralSettings.closeToTray).toBool();
	m_generalSettings.checkForUpdatesOnStartup = m_settings.value("general/checkForUpdatesOnStartup", m_defaultGeneralSettings.checkForUpdatesOnStartup).toBool();
	m_generalSettings.language = m_settings.value("general/language", QString::fromStdString(m_defaultGeneralSettings.language)).toString().toStdString();

	// Advanced settings
	m_advancedSettings.enableLogging = m_settings.value("advanced/enableLogging", m_defaultAdvancedSettings.enableLogging).toBool();
	m_advancedSettings.enableDebugMode = m_settings.value("advanced/enableDebugMode", m_defaultAdvancedSettings.enableDebugMode).toBool();

	// Session settings
	m_sessionSettings.outputDirectory = QDir(m_settings.value("session/outputDirectory", m_defaultSessionSettings.outputDirectory.absolutePath()).toString());
	m_sessionSettings.outputPrefix = m_settings.value("session/outputPrefix", QString::fromStdString(m_defaultSessionSettings.outputPrefix)).toString().toStdString();
	m_sessionSettings.enableClipping = m_settings.value("session/enableClipping", m_defaultSessionSettings.enableClipping).toBool();

	m_settings.endGroup();
}

void SettingsController::saveSettings()
{
	m_settings.beginGroup(MSS_SETTINGS_GROUP_BACKEND); // Core settings group

	// General settings
	m_settings.setValue("general/closeToTray", m_generalSettings.closeToTray);
	m_settings.setValue("general/checkForUpdatesOnStartup", m_generalSettings.checkForUpdatesOnStartup);
	m_settings.setValue("general/language", QString::fromStdString(m_generalSettings.language));

	// Advanced settings
	m_settings.setValue("advanced/enableLogging", m_advancedSettings.enableLogging);
	m_settings.setValue("advanced/enableDebugMode", m_advancedSettings.enableDebugMode);

	// Session settings
	m_settings.setValue("session/outputDirectory", m_sessionSettings.outputDirectory.absolutePath());
	m_settings.setValue("session/outputPrefix", QString::fromStdString(m_sessionSettings.outputPrefix));
	m_settings.setValue("session/enableClipping", m_sessionSettings.enableClipping);

	m_settings.endGroup();
}
