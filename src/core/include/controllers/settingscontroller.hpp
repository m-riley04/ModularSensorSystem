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

	GeneralSettings& generalSettings() { return m_generalSettings; }
	AdvancedSettings& advancedSettings() { return m_advancedSettings; }
	PresetSettings& presetSettings() { return m_presetSettings; }
	SessionSettings& sessionSettings() { return m_sessionSettings; }
	PluginsSettings& pluginsSettings() { return m_pluginsSettings; }

public slots:
	void loadSettings();
	void saveSettings();
	void restoreDefaultSettings() {
		restoreDefaultGeneralSettings();
		restoreDefaultAdvancedSettings();
		restoreDefaultPresetSettings();
		restoreDefaultSessionSettings();
		restoreDefaultPluginsSettings();
	}

	// General settings setters
	void setGeneralSettings(const GeneralSettings& settings) { m_generalSettings = settings; }
	void setCloseToTray(bool closeToTray) { m_generalSettings.closeToTray = closeToTray; }
	void setCheckForUpdatesOnStartup(bool check) { m_generalSettings.checkForUpdatesOnStartup = check; }
	void setLanguage(const QString& language) { m_generalSettings.language = language; }

	// Advanced settings setters
	void setAdvancedSettings(const AdvancedSettings& settings) { m_advancedSettings = settings; }
	void setEnableLogging(bool enabled) { m_advancedSettings.enableLogging = enabled; }
	void setEnableDebugMode(bool enabled) { m_advancedSettings.enableDebugMode = enabled; }

	// PReset settings setters
	void setPresetSettings(const PresetSettings& settings) { m_presetSettings = settings; }
	void setPresetDirectory(const QDir& dir) { m_presetSettings.presetDirectory = dir; }

	// Session settings setters
	void setSessionSettings(const SessionSettings& settings) { m_sessionSettings = settings; }
	void setOutputDirectory(const QDir& dir) { m_sessionSettings.outputDirectory = dir; }
	void setOutputPrefix(const QString& prefix) { m_sessionSettings.outputPrefix = prefix; }
	void setEnableClipping(bool enabled) { m_sessionSettings.enableClipping = enabled; }

	// Plugins settings setters
	void setPluginsSettings(const PluginsSettings& settings) { m_pluginsSettings = settings; }
	void setPluginsDirectory(const QDir& dir) { m_pluginsSettings.pluginsDirectory = dir; }
	void setEnabledPluginIds(const QStringList& ids) { m_pluginsSettings.enabledPluginIds = ids; }
	void addEnabledPluginId(const QString& id) { m_pluginsSettings.enabledPluginIds.push_back(id); }
	void removeEnabledPluginId(const QString& id) {
		m_pluginsSettings.enabledPluginIds.erase(
			std::remove(m_pluginsSettings.enabledPluginIds.begin(), m_pluginsSettings.enabledPluginIds.end(), id),
			m_pluginsSettings.enabledPluginIds.end()
		);
	}

private:
	void restoreDefaultGeneralSettings() { m_generalSettings = m_defaultGeneralSettings; }
	void restoreDefaultAdvancedSettings() { m_advancedSettings = m_defaultAdvancedSettings; }
	void restoreDefaultPresetSettings() { m_presetSettings = m_defaultPresetSettings; }
	void restoreDefaultSessionSettings() { m_sessionSettings = m_defaultSessionSettings; }
	void restoreDefaultPluginsSettings() { m_pluginsSettings = m_defaultPluginsSettings; }

private:
	QSettings& m_settings;
	GeneralSettings m_generalSettings;
	AdvancedSettings m_advancedSettings;
	SessionSettings m_sessionSettings;
	PluginsSettings m_pluginsSettings;
	PresetSettings m_presetSettings;

	GeneralSettings m_defaultGeneralSettings; // TODO: should these be stored elsewhere? Very likely
	AdvancedSettings m_defaultAdvancedSettings;
	SessionSettings m_defaultSessionSettings;
	PluginsSettings m_defaultPluginsSettings;
	PresetSettings m_defaultPresetSettings;

};