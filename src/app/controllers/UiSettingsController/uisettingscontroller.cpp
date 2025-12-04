#include "uisettingscontroller.h"

UiSettingsController::UiSettingsController(QSettings& settings, QObject *parent)
	: QObject(parent), m_settings(settings)
{
	// Init defaults
	m_defaultWindowSettings = WindowSettings();
	m_defaultAccessibilitySettings = AccessibilitySettings();
	m_defaultKeybindingSettings = KeybindingSettings();
	m_defaultAppearanceSettings = AppearanceSettings();
}

UiSettingsController::~UiSettingsController()
{}

void UiSettingsController::loadSettings()
{
	m_settings.beginGroup(MSS_SETTINGS_GROUP_FRONTEND); // UI settings group

	// Window
	m_windowSettings.geometry = m_settings.value("window/geometry", QByteArray()).toByteArray();
	m_windowSettings.windowState = m_settings.value("window/windowState", QByteArray()).toByteArray();

	// Appearance
	m_appearanceSettings.theme = m_settings.value("appearance/theme", m_defaultAppearanceSettings.theme).toString();

	// Keybindings
	m_keybindingSettings.toggleSession = m_settings.value("keybindings/toggleSession", m_defaultKeybindingSettings.toggleSession).value<QKeySequence>();
	m_keybindingSettings.toggleRecording = m_settings.value("keybindings/toggleRecording", m_defaultKeybindingSettings.toggleRecording).value<QKeySequence>();
	m_keybindingSettings.clipSession = m_settings.value("keybindings/clipSession", m_defaultKeybindingSettings.clipSession).value<QKeySequence>();

	// Accessibility
	// TODO

	m_settings.endGroup();
}

void UiSettingsController::saveSettings()
{
	m_settings.beginGroup(MSS_SETTINGS_GROUP_FRONTEND); // UI settings group

	// Window
	m_settings.setValue("window/geometry", m_windowSettings.geometry);
	m_settings.setValue("window/windowState", m_windowSettings.windowState);
	
	// Appearance
	m_settings.setValue("appearance/theme", m_appearanceSettings.theme);

	// Keybindings
	m_settings.setValue("keybindings/toggleSession", m_keybindingSettings.toggleSession);
	m_settings.setValue("keybindings/toggleRecording", m_keybindingSettings.toggleRecording);
	m_settings.setValue("keybindings/clipSession", m_keybindingSettings.clipSession);

	// Accessibility
	// TODO

	m_settings.endGroup();
}

void UiSettingsController::restoreDefaultSettings()
{
	m_windowSettings = m_defaultWindowSettings;
	m_appearanceSettings = m_defaultAppearanceSettings;
	m_accessibilitySettings = m_defaultAccessibilitySettings;
}