#pragma once

#include <QDir>
#include <QCoreApplication>
#include <string>
#include <QSize>

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
	// General session settings
	bool confirmOnStopSession = false;
	bool confirmOnStopRecording = false;

	// Recording settings
	QDir outputDirectory = QCoreApplication::applicationDirPath() + "/recordings";
	QString outputPrefix = "session_";
	bool overwriteExistingFiles = false;
	bool allowSpacesInFilenames = true;

	// Clipping settings
	bool enableClipping = false;
};

struct SourcesSettings
{
	// Video sources
	QSize defaultVideoResolution = QSize(1920, 1080);
	double defaultVideoFramerate = 30.0;
	int defaultVideoBitrateKbps = 5000;
	QString defaultPixelFormat = "yuv420p";
	QString defaultVideoCodec = "H.264";
	QString defaultVideoContainerFormat = "mp4";

	// Audio sources
	int defaultAudioSampleRate = 44100;
	bool defaultAudioStereo = true;
	QString defaultAudioContainerFormat = "aac";
	QString defaultAudioCodec = "AAC";

	// Data sources
	QString defaultDataFormat = "json";
};

struct MountSettings
{
	// TODO: add mount-related settings here
};

struct ProcessorSettings
{
	// TODO: add processor-related settings here
};

struct PresetSettings
{
	QDir presetDirectory = QCoreApplication::applicationDirPath() + "/presets";
};

struct PluginsSettings
{
	QDir pluginsDirectory = QCoreApplication::applicationDirPath() + "/plugins";
	QStringList enabledPluginIds = {""};
};
