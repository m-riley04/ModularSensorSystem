#pragma once

#include <QDir>
#include <QCoreApplication>
#include <string>

struct GeneralSettings
{
	bool closeToTray = true;
	bool checkForUpdatesOnStartup = true;
	QString language = "system"; // "system" to use system language by default
};

struct AdvancedSettings
{
	bool enableLogging = false;
	bool enableDebugMode = false;
};

struct SessionSettings
{
	// Recording settings
	QDir outputDirectory = QCoreApplication::applicationDirPath() + "/recordings";
	QString outputPrefix = "session_";

	// Clipping settings
	bool enableClipping = false;
};

struct PluginsSettings
{
	QDir pluginsDirectory = QCoreApplication::applicationDirPath() + "/plugins";
	QStringList enabledPluginIds = {};
};

struct PresetSettings
{
	QDir presetDirectory = QCoreApplication::applicationDirPath() + "/presets";
};