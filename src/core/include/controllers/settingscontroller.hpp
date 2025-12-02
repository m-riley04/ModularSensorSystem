#pragma once

#include <QObject>
#include <QSettings>
#include <QDir>
#include <QCoreApplication>

constexpr const char* MSS_SETTINGS_GROUP_BACKEND = "core";

struct AdvancedSettings
{
	bool enableLogging = false;
	bool enableDebugMode = false;
};

struct SessionSettings
{
	// Recording settings
	QDir outputDirectory = QCoreApplication::applicationDirPath() + "/recordings";
	std::string outputPrefix = "session_";

	// Clipping settings
	bool enableClipping = false;
};

struct GeneralSettings
{
	bool closeToTray = true;
	bool checkForUpdatesOnStartup = true;
	std::string language = "system"; // "system" to use system language by default
};

class SettingsController : public QObject
{
	Q_OBJECT

public:
	SettingsController(QSettings& settings, QObject* parent = nullptr);
	~SettingsController();

	GeneralSettings generalSettings() const { return m_generalSettings; }
	AdvancedSettings advancedSettings() const { return m_advancedSettings; }
	SessionSettings sessionSettings() const { return m_sessionSettings; }

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