#pragma once

#include <QDir>
#include <QCoreApplication>
#include <string>

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