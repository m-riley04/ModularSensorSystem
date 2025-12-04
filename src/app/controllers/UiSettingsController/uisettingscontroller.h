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
	QString theme = "default";
};

struct KeybindingSettings
{
	// Session keybindings
	QKeySequence toggleSession = QKeySequence("Ctrl+Shift+S");
	QKeySequence toggleRecording = QKeySequence("Ctrl+R");
	QKeySequence clipSession = QKeySequence("Ctrl+C"); // TODO: change these defaults, they suck
};

struct AccessibilitySettings
{
	// TODO: Add accessibility-related settings here
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
	KeybindingSettings keybindingSettings() const { return m_keybindingSettings; }
	AccessibilitySettings accessibilitySettings() const { return m_accessibilitySettings; }

public slots:
	void loadSettings();
	void saveSettings();
	void restoreDefaultSettings();

	// Window settings setters
	void setWindowSettings(const WindowSettings& settings) { m_windowSettings = settings; }

	// Appearance settings setters
	void setAppearanceSettings(const AppearanceSettings& settings) { m_appearanceSettings = settings; }
	void setTheme(const QString& theme) { m_appearanceSettings.theme = theme; }

	// Keybinding settings setters
	void setKeybindingSettings(const KeybindingSettings& settings) { m_keybindingSettings = settings; }
	void setToggleSessionKeybinding(const QKeySequence& keybinding) { m_keybindingSettings.toggleSession = keybinding; }
	void setToggleRecordingKeybinding(const QKeySequence& keybinding) { m_keybindingSettings.toggleRecording = keybinding; }
	void setClipSessionKeybinding(const QKeySequence& keybinding) { m_keybindingSettings.clipSession = keybinding; }

	// Accessibility settings setters
	void setAccessibilitySettings(const AccessibilitySettings& settings) { m_accessibilitySettings = settings; }

private:
	QSettings& m_settings;
	WindowSettings m_windowSettings;
	AppearanceSettings m_appearanceSettings;
	KeybindingSettings m_keybindingSettings;
	AccessibilitySettings m_accessibilitySettings;

	WindowSettings m_defaultWindowSettings;
	AppearanceSettings m_defaultAppearanceSettings;
	KeybindingSettings m_defaultKeybindingSettings;
	AccessibilitySettings m_defaultAccessibilitySettings;
};

