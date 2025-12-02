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
	SessionSettings& sessionSettings() { return m_sessionSettings; }

public slots:
	void loadSettings();
	void saveSettings();
	void restoreDefaultSettings() {
		restoreDefaultGeneralSettings();
		restoreDefaultAdvancedSettings();
		restoreDefaultSessionSettings();
	}

	// General settings setters
	void setGeneralSettings(const GeneralSettings& settings) { m_generalSettings = settings; }
	void setCloseToTray(bool closeToTray) { m_generalSettings.closeToTray = closeToTray; }
	void setCheckForUpdatesOnStartup(bool check) { m_generalSettings.checkForUpdatesOnStartup = check; }
	void setLanguage(const QString& language) { m_generalSettings.language = language.toStdString(); }

	// Session settings setters
	void setSessionSettings(const SessionSettings& settings) { m_sessionSettings = settings; }
	void setOutputDirectory(const QDir& dir) { m_sessionSettings.outputDirectory = dir; }
	void setOutputPrefix(const QString& prefix) { m_sessionSettings.outputPrefix = prefix.toStdString(); }
	void setEnableClipping(bool enabled) { m_sessionSettings.enableClipping = enabled; }

	// Advanced settings setters
	void setAdvancedSettings(const AdvancedSettings& settings) { m_advancedSettings = settings; }
	void setEnableLogging(bool enabled) { m_advancedSettings.enableLogging = enabled; }
	void setEnableDebugMode(bool enabled) { m_advancedSettings.enableDebugMode = enabled; }

private:
	void restoreDefaultGeneralSettings() { m_generalSettings = m_defaultGeneralSettings; }
	void restoreDefaultAdvancedSettings() { m_advancedSettings = m_defaultAdvancedSettings; }
	void restoreDefaultSessionSettings() { m_sessionSettings = m_defaultSessionSettings; }

private:
	QSettings& m_settings;
	GeneralSettings m_generalSettings;
	AdvancedSettings m_advancedSettings;
	SessionSettings m_sessionSettings;

	GeneralSettings m_defaultGeneralSettings;
	AdvancedSettings m_defaultAdvancedSettings;
	SessionSettings m_defaultSessionSettings;
};