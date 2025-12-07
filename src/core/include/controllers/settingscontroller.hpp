#pragma once

#include <QObject>
#include <QSettings>
#include "models/settings_models.hpp"

constexpr const char* MSS_SETTINGS_GROUP_BACKEND = "core";

class SettingsController : public QObject
{
	Q_OBJECT

public:
	SettingsController(QSettings& settings, QObject* parent = nullptr);
	~SettingsController();

	QSettings& settings() { return m_settings; }

	GeneralSettings& generalSettings() { return m_generalSettings; }
	AdvancedSettings& advancedSettings() { return m_advancedSettings; }
	SessionSettings& sessionSettings() { return m_sessionSettings; }
	SourcesSettings& sourcesSettings() { return m_sourcesSettings; }
	MountSettings& mountSettings() { return m_mountSettings; }
	ProcessorSettings& processorSettings() { return m_processorSettings; }
	PresetSettings& presetSettings() { return m_presetSettings; }
	PluginsSettings& pluginsSettings() { return m_pluginsSettings; }

signals:
	void enableLoggingChanged(bool enabled);
	void useUniqueLogFilesChanged(bool unique);

public slots:
	void loadSettings();
	void saveSettings();
	void restoreDefaultSettings() {
		restoreDefaultGeneralSettings();
		restoreDefaultAdvancedSettings();
		restoreDefaultSessionSettings();
		restoreDefaultSourcesSettings();
		restoreDefaultMountSettings();
		restoreDefaultProcessorSettings();
		restoreDefaultPresetSettings();
		restoreDefaultPluginsSettings();
	}

	// General settings setters
	void setGeneralSettings(const GeneralSettings& settings) { m_generalSettings = settings; }
	void setCloseToTray(bool closeToTray) { m_generalSettings.closeToTray = closeToTray; }
	void setCheckForUpdatesOnStartup(bool check) { m_generalSettings.checkForUpdatesOnStartup = check; }
	void setLanguage(const QString& language) { m_generalSettings.language = language; }

	// Advanced settings setters
	void setAdvancedSettings(const AdvancedSettings& settings) { m_advancedSettings = settings; }
	void setEnableDebugMode(bool enabled) { m_advancedSettings.enableDebugMode = enabled; }
	void setEnableLogging(bool enabled) {
		if (m_advancedSettings.enableLogging == enabled) return;
		m_advancedSettings.enableLogging = enabled;
		emit enableLoggingChanged(enabled);
	}
	void setLogDirectory(const QDir& dir) { m_advancedSettings.logDirectory = dir; }
	void setUseUniqueLogFiles(bool unique) {
		if (m_advancedSettings.useUniqueLogFiles == unique) return;
		m_advancedSettings.useUniqueLogFiles = unique;
		emit useUniqueLogFilesChanged(unique);
	}

	// Session settings setters
	void setSessionSettings(const SessionSettings& settings) { m_sessionSettings = settings; }
	void setConfirmOnStopSession(bool confirm) { m_sessionSettings.confirmOnStopSession = confirm; }
	void setConfirmOnStopRecording(bool confirm) { m_sessionSettings.confirmOnStopRecording = confirm; }
	void setOutputDirectory(const QDir& dir) { m_sessionSettings.outputDirectory = dir; }
	void setOutputPrefix(const QString& prefix) { m_sessionSettings.outputPrefix = prefix; }
	void setOverwriteExistingFiles(bool overwrite) { m_sessionSettings.overwriteExistingFiles = overwrite; }
	void setAllowSpacesInFilenames(bool allow) { m_sessionSettings.allowSpacesInFilenames = allow; }
	void setEnableClipping(bool enabled) { m_sessionSettings.enableClipping = enabled; }

	// Sources settings setters
	void setSourcesSettings(const SourcesSettings& settings) { m_sourcesSettings = settings; }
	// Video sources defaults
	void setDefaultVideoResolution(const QSize& resolution) { m_sourcesSettings.defaultVideoResolution = resolution; }
	void setDefaultVideoFramerate(double framerate) { m_sourcesSettings.defaultVideoFramerate = framerate; }
	void setDefaultVideoBitrateKbps(int bitrateKbps) { m_sourcesSettings.defaultVideoBitrateKbps = bitrateKbps; }
	void setDefaultVideoContainerFormat(const QString& format) { m_sourcesSettings.defaultVideoContainerFormat = format; }
	void setDefaultPixelFormat(const QString& format) { m_sourcesSettings.defaultPixelFormat = format; }
	void setDefaultVideoCodec(const QString& codec) { m_sourcesSettings.defaultVideoCodec = codec; }
	// Audio sources defaults
	void setDefaultAudioSampleRate(int sampleRate) { m_sourcesSettings.defaultAudioSampleRate = sampleRate; }
	void setDefaultAudioStereo(bool stereo) { m_sourcesSettings.defaultAudioStereo = stereo; }
	void setDefaultAudioContainerFormat(const QString& format) { m_sourcesSettings.defaultAudioContainerFormat = format; }
	void setDefaultAudioCodec(const QString& codec) { m_sourcesSettings.defaultAudioCodec = codec; }
	// Data sources defaults
	void setDefaultDataFormat(const QString& format) { m_sourcesSettings.defaultDataFormat = format; }

	// Mount settings setters
	// TODO

	// Processor settings setters
	// TODO

	// Preset settings setters
	void setPresetSettings(const PresetSettings& settings) { m_presetSettings = settings; }
	void setPresetDirectory(const QDir& dir) { m_presetSettings.presetDirectory = dir; }

	// Plugins settings setters
	void setPluginsSettings(const PluginsSettings& settings) { m_pluginsSettings = settings; }
	void setPluginsDirectory(const QDir& dir) { m_pluginsSettings.pluginsDirectory = dir; }
	void setEnabledPluginIds(const QStringList& ids) { m_pluginsSettings.enabledPluginIds = ids; }
	void addEnabledPluginId(const QString& id) { m_pluginsSettings.enabledPluginIds.append(id); }
	void removeEnabledPluginId(const QString& id) {
		m_pluginsSettings.enabledPluginIds.erase(
			std::remove(m_pluginsSettings.enabledPluginIds.begin(), m_pluginsSettings.enabledPluginIds.end(), id),
			m_pluginsSettings.enabledPluginIds.end()
		);
	}

private:
	void restoreDefaultGeneralSettings() { m_generalSettings = m_defaultGeneralSettings; }
	void restoreDefaultAdvancedSettings() { m_advancedSettings = m_defaultAdvancedSettings; }
	void restoreDefaultSessionSettings() { m_sessionSettings = m_defaultSessionSettings; }
	void restoreDefaultSourcesSettings() { m_sourcesSettings = m_defaultSourcesSettings; }
	void restoreDefaultMountSettings() { m_mountSettings = m_defaultMountSettings; }
	void restoreDefaultProcessorSettings() { m_processorSettings = m_defaultProcessorSettings; }
	void restoreDefaultPresetSettings() { m_presetSettings = m_defaultPresetSettings; }
	void restoreDefaultPluginsSettings() { m_pluginsSettings = m_defaultPluginsSettings; }

private:
	QSettings& m_settings;
	GeneralSettings m_generalSettings;
	AdvancedSettings m_advancedSettings;
	SessionSettings m_sessionSettings;
	SourcesSettings m_sourcesSettings;
	MountSettings m_mountSettings;
	ProcessorSettings m_processorSettings;
	PluginsSettings m_pluginsSettings;
	PresetSettings m_presetSettings;

	GeneralSettings m_defaultGeneralSettings; // TODO: should these be stored elsewhere? Very likely
	AdvancedSettings m_defaultAdvancedSettings;
	SessionSettings m_defaultSessionSettings;
	SourcesSettings m_defaultSourcesSettings;
	MountSettings m_defaultMountSettings;
	ProcessorSettings m_defaultProcessorSettings;
	PluginsSettings m_defaultPluginsSettings;
	PresetSettings m_defaultPresetSettings;

};