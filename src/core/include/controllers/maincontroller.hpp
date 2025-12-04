#pragma once

#include <QObject>
#include <QThread>
#include <QSettings>
#include <qcoreapplication.h>
#include <memory>
#include "controllers/plugincontroller.hpp"
#include "controllers/presetscontroller.hpp"
#include "controllers/processingcontroller.hpp"
#include "controllers/sourcecontroller.hpp"
#include "controllers/mountcontroller.hpp"
#include "controllers/sessioncontroller.hpp"
#include "controllers/settingscontroller.hpp"

class MainController  : public QObject
{
	Q_OBJECT

public:
	MainController(QSettings& settings, QObject *parent);
	~MainController();

	SourceController& sourceController() { return m_sourceController; }
	PluginController& pluginController() { return m_pluginController; }
	PresetsController& presetsController() { return m_presetsController; }
	ProcessingController& processingController() { return m_processingController; }
	MountController& mountController() { return m_mountController; }
	SessionController& sessionController() { return m_sessionController; }
	SettingsController& settingsController() { return m_settingsController; }

private:
	// IMPORTANT: Construct settings first so dependent controllers receive a valid reference
	SettingsController m_settingsController;
	SourceController m_sourceController;
	ProcessingController m_processingController;
	MountController m_mountController;
	PluginController m_pluginController;
	PresetsController m_presetsController;
	SessionController m_sessionController;
};
