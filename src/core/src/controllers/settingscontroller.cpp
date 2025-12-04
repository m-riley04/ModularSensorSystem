#include "controllers/settingscontroller.hpp"

SettingsController::SettingsController(QSettings& settings, QObject* parent)
	: QObject(parent), m_settings(settings)
{
	// Initialize default settings
	m_defaultGeneralSettings = GeneralSettings();
	m_defaultAdvancedSettings = AdvancedSettings();
	m_defaultSessionSettings = SessionSettings();
	m_defaultSourcesSettings = SourcesSettings();
	m_defaultMountSettings = MountSettings();
	m_defaultProcessorSettings = ProcessorSettings();
	m_defaultPresetSettings = PresetSettings();
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
	m_sessionSettings.confirmOnStopSession = m_settings.value("session/confirmOnStopSession", m_defaultSessionSettings.confirmOnStopSession).toBool();
	m_sessionSettings.confirmOnStopRecording = m_settings.value("session/confirmOnStopRecording", m_defaultSessionSettings.confirmOnStopRecording).toBool();
	m_sessionSettings.outputDirectory = QDir(m_settings.value("session/outputDirectory", m_defaultSessionSettings.outputDirectory.absolutePath()).toString());
	m_sessionSettings.outputPrefix = m_settings.value("session/outputPrefix", m_defaultSessionSettings.outputPrefix).toString();
	m_sessionSettings.overwriteExistingFiles = m_settings.value("session/overwriteExistingFiles", m_defaultSessionSettings.overwriteExistingFiles).toBool();
	m_sessionSettings.allowSpacesInFilenames = m_settings.value("session/allowSpacesInFilenames", m_defaultSessionSettings.allowSpacesInFilenames).toBool();
	m_sessionSettings.enableClipping = m_settings.value("session/enableClipping", m_defaultSessionSettings.enableClipping).toBool();

	// Source settings
	m_sourcesSettings.defaultVideoResolution = m_settings.value("sources/defaultVideoResolution", m_defaultSourcesSettings.defaultVideoResolution).toSize();
	m_sourcesSettings.defaultVideoFramerate = m_settings.value("sources/defaultVideoFramerate", m_defaultSourcesSettings.defaultVideoFramerate).toDouble();
	m_sourcesSettings.defaultVideoBitrateKbps = m_settings.value("sources/defaultVideoBitrateKbps", m_defaultSourcesSettings.defaultVideoBitrateKbps).toInt();
	m_sourcesSettings.defaultPixelFormat = m_settings.value("sources/defaultPixelFormat", m_defaultSourcesSettings.defaultPixelFormat).toString();
	m_sourcesSettings.defaultVideoCodec = m_settings.value("sources/defaultVideoCodec", m_defaultSourcesSettings.defaultVideoCodec).toString();
	m_sourcesSettings.defaultVideoContainerFormat = m_settings.value("sources/defaultVideoContainerFormat", m_defaultSourcesSettings.defaultVideoContainerFormat).toString();
	// audio sources
	m_sourcesSettings.defaultAudioSampleRate = m_settings.value("sources/defaultAudioSampleRate", m_defaultSourcesSettings.defaultAudioSampleRate).toInt();
	m_sourcesSettings.defaultAudioStereo = m_settings.value("sources/defaultAudioStereo", m_defaultSourcesSettings.defaultAudioStereo).toBool();
	m_sourcesSettings.defaultAudioContainerFormat = m_settings.value("sources/defaultAudioContainerFormat", m_defaultSourcesSettings.defaultAudioContainerFormat).toString();
	m_sourcesSettings.defaultAudioCodec = m_settings.value("sources/defaultAudioCodec", m_defaultSourcesSettings.defaultAudioCodec).toString();
	// data sources
	m_sourcesSettings.defaultDataFormat = m_settings.value("sources/defaultDataFormat", m_defaultSourcesSettings.defaultDataFormat).toString();

	// Mount settings
	// TODO

	// Processor settings
	// TODO

	// Preset settings
	m_presetSettings.presetDirectory = QDir(m_settings.value("preset/presetDirectory", m_defaultPresetSettings.presetDirectory.absolutePath()).toString());

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
	m_settings.setValue("session/confirmOnStopSession", m_sessionSettings.confirmOnStopSession);
	m_settings.setValue("session/confirmOnStopRecording", m_sessionSettings.confirmOnStopRecording);
	m_settings.setValue("session/outputDirectory", m_sessionSettings.outputDirectory.absolutePath());
	m_settings.setValue("session/outputPrefix", m_sessionSettings.outputPrefix);
	m_settings.setValue("session/overwriteExistingFiles", m_sessionSettings.overwriteExistingFiles);
	m_settings.setValue("session/allowSpacesInFilenames", m_sessionSettings.allowSpacesInFilenames);
	m_settings.setValue("session/enableClipping", m_sessionSettings.enableClipping);

	// Source settings
	// video sources
	m_settings.setValue("sources/defaultVideoResolution", m_sourcesSettings.defaultVideoResolution);
	m_settings.setValue("sources/defaultVideoFramerate", m_sourcesSettings.defaultVideoFramerate);
	m_settings.setValue("sources/defaultVideoBitrateKbps", m_sourcesSettings.defaultVideoBitrateKbps);
	m_settings.setValue("sources/defaultPixelFormat", m_sourcesSettings.defaultPixelFormat);
	m_settings.setValue("sources/defaultVideoCodec", m_sourcesSettings.defaultVideoCodec);
	m_settings.setValue("sources/defaultVideoContainerFormat", m_sourcesSettings.defaultVideoContainerFormat);
	// audio sources
	m_settings.setValue("sources/defaultAudioSampleRate", m_sourcesSettings.defaultAudioSampleRate);
	m_settings.setValue("sources/defaultAudioStereo", m_sourcesSettings.defaultAudioStereo);
	m_settings.setValue("sources/defaultAudioContainerFormat", m_sourcesSettings.defaultAudioContainerFormat);
	m_settings.setValue("sources/defaultAudioCodec", m_sourcesSettings.defaultAudioCodec);
	// data sources
	m_settings.setValue("sources/defaultDataFormat", m_sourcesSettings.defaultDataFormat);

	// Mount settings
	// TODO

	// Processor settings
	// TODO

	// Preset settings
	m_settings.setValue("preset/presetDirectory", m_presetSettings.presetDirectory.absolutePath());
	
	// Plugins settings
	m_settings.setValue("plugins/pluginsDirectory", m_pluginsSettings.pluginsDirectory.absolutePath());
	m_settings.setValue("plugins/enabledPluginIds", m_pluginsSettings.enabledPluginIds);

	m_settings.endGroup();
}
