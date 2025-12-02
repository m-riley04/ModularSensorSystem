#include "controllers/settingscontroller.hpp"

SettingsController::SettingsController(QSettings& settings, QObject* parent)
	: QObject(parent), m_settings(settings)
{
	// Initialize default settings
	m_defaultGeneralSettings = GeneralSettings();
	m_defaultAdvancedSettings = AdvancedSettings();
	m_defaultPresetSettings = PresetSettings();
	m_defaultSessionSettings = SessionSettings();
	m_defaultPluginsSettings = PluginsSettings();

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
	m_generalSettings.language = m_settings.value("general/language", m_defaultGeneralSettings.language).toString();

	// Advanced settings
	m_advancedSettings.enableLogging = m_settings.value("advanced/enableLogging", m_defaultAdvancedSettings.enableLogging).toBool();
	m_advancedSettings.enableDebugMode = m_settings.value("advanced/enableDebugMode", m_defaultAdvancedSettings.enableDebugMode).toBool();

	// Preset settings
	m_presetSettings.presetDirectory = QDir(m_settings.value("preset/presetDirectory", m_defaultPresetSettings.presetDirectory.absolutePath()).toString());

	// Session settings
	m_sessionSettings.outputDirectory = QDir(m_settings.value("session/outputDirectory", m_defaultSessionSettings.outputDirectory.absolutePath()).toString());
	m_sessionSettings.outputPrefix = m_settings.value("session/outputPrefix", m_defaultSessionSettings.outputPrefix).toString();
	m_sessionSettings.enableClipping = m_settings.value("session/enableClipping", m_defaultSessionSettings.enableClipping).toBool();

	// Plugins settings
	m_pluginsSettings.pluginsDirectory = QDir(m_settings.value("plugins/pluginsDirectory", m_defaultPluginsSettings.pluginsDirectory.absolutePath()).toString());
	m_pluginsSettings.enabledPluginIds = m_settings.value("plugins/enabledPluginIds").toStringList();

	m_settings.endGroup();
}

void SettingsController::saveSettings()
{
	m_settings.beginGroup(MSS_SETTINGS_GROUP_BACKEND); // Core settings group

	// General settings
	m_settings.setValue("general/closeToTray", m_generalSettings.closeToTray);
	m_settings.setValue("general/checkForUpdatesOnStartup", m_generalSettings.checkForUpdatesOnStartup);
	m_settings.setValue("general/language", m_generalSettings.language);

	// Advanced settings
	m_settings.setValue("advanced/enableLogging", m_advancedSettings.enableLogging);
	m_settings.setValue("advanced/enableDebugMode", m_advancedSettings.enableDebugMode);

	// Preset settings
	m_settings.setValue("preset/presetDirectory", m_presetSettings.presetDirectory.absolutePath());

	// Session settings
	m_settings.setValue("session/outputDirectory", m_sessionSettings.outputDirectory.absolutePath());
	m_settings.setValue("session/outputPrefix", m_sessionSettings.outputPrefix);
	m_settings.setValue("session/enableClipping", m_sessionSettings.enableClipping);
	
	// Plugins settings
	m_settings.setValue("plugins/pluginsDirectory", m_pluginsSettings.pluginsDirectory.absolutePath());
	m_settings.setValue("plugins/enabledPluginIds", m_pluginsSettings.enabledPluginIds);

	m_settings.endGroup();
}
