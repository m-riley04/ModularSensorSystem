#pragma once

#include <QObject>
#include <QSettings>

constexpr const char* MSS_SETTINGS_GROUP_BACKEND = "core";

struct AdvancedSettings
{

};

struct SourceSettings
{

};

struct SessionSettings
{
	
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
	SourceSettings sourceSettings() const { return m_sourceSettings; }
	SessionSettings sessionSettings() const { return m_sessionSettings; }

	void setGeneralSettings(const GeneralSettings& settings) { m_generalSettings = settings; }
	void setAdvancedSettings(const AdvancedSettings& settings) { m_advancedSettings = settings; }
	void setSourceSettings(const SourceSettings& settings) { m_sourceSettings = settings; }
	void setSessionSettings(const SessionSettings& settings) { m_sessionSettings = settings; }

public slots:
	void loadSettings();
	void saveSettings();

private:
	QSettings& m_settings;
	GeneralSettings m_generalSettings;
	AdvancedSettings m_advancedSettings;
	SourceSettings m_sourceSettings;
	SessionSettings m_sessionSettings;

	GeneralSettings m_defaultGeneralSettings;
	AdvancedSettings m_defaultAdvancedSettings;
	SourceSettings m_defaultSourceSettings;
	SessionSettings m_defaultSessionSettings;
};