#include "uisettingscontroller.h"

UiSettingsController::UiSettingsController(QSettings& settings, QObject *parent)
	: QObject(parent), m_settings(settings)
{
	// Init defaults
	m_defaultWindowSettings = WindowSettings();
	m_defaultAccessibilitySettings = AccessibilitySettings();
	m_defaultAppearanceSettings = AppearanceSettings();
}

UiSettingsController::~UiSettingsController()
{}

void UiSettingsController::loadSettings()
{
	m_settings.beginGroup(MSS_SETTINGS_GROUP_FRONTEND); // UI settings group

	m_windowSettings.geometry = m_settings.value("window/geometry", QByteArray()).toByteArray();
	m_windowSettings.windowState = m_settings.value("window/windowState", QByteArray()).toByteArray();

	m_settings.endGroup();
}

void UiSettingsController::saveSettings()
{
	m_settings.beginGroup(MSS_SETTINGS_GROUP_FRONTEND); // UI settings group

	m_settings.setValue("window/geometry", m_windowSettings.geometry);
	m_settings.setValue("window/windowState", m_windowSettings.windowState);

	m_settings.endGroup();
}

void UiSettingsController::restoreDefaultSettings()
{
	m_windowSettings = m_defaultWindowSettings;
	m_appearanceSettings = m_defaultAppearanceSettings;
	m_accessibilitySettings = m_defaultAccessibilitySettings;
}