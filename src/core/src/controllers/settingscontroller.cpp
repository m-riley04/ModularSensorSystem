#include "controllers/settingscontroller.hpp"

SettingsController::SettingsController(QSettings& settings, QObject* parent)
	: QObject(parent), m_settings(settings)
{
	// Initialize default settings
	m_defaultGeneralSettings = GeneralSettings();
	m_defaultAdvancedSettings = AdvancedSettings();
	m_defaultSourceSettings = SourceSettings();
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
	// TODO: Load advanced settings here

	// Source settings
	// TODO: Load source settings here

	// Session settings
	// TODO: Load session settings here

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
	// TODO

	// Source settings
	// TODO

	// Session settings
	// TODO

	m_settings.endGroup();
}
