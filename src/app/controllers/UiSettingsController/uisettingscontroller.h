#pragma once

#include <QObject>
#include <QWidget>
#include <QSettings>

constexpr const char* MSS_SETTINGS_GROUP_FRONTEND = "ui";

struct WindowSettings
{
	QByteArray geometry;
	QByteArray windowState;
};

struct AppearanceSettings
{

};

struct AccessibilitySettings
{

};

/**
 * @brief The controller for UI-specific settings.
 */
class UiSettingsController  : public QObject
{
	Q_OBJECT

public:
	UiSettingsController(QSettings& settings, QObject *parent);
	~UiSettingsController();

	WindowSettings windowSettings() const { return m_windowSettings; }
	AppearanceSettings appearanceSettings() const { return m_appearanceSettings; }
	AccessibilitySettings accessibilitySettings() const { return m_accessibilitySettings; }

	void setWindowSettings(const WindowSettings& settings) { m_windowSettings = settings; }
	void setAppearanceSettings(const AppearanceSettings& settings) { m_appearanceSettings = settings; }
	void setAccessibilitySettings(const AccessibilitySettings& settings) { m_accessibilitySettings = settings; }

public slots:
	void loadSettings();
	void saveSettings();

private:
	QSettings& m_settings;
	WindowSettings m_windowSettings;
	AppearanceSettings m_appearanceSettings;
	AccessibilitySettings m_accessibilitySettings;
};

