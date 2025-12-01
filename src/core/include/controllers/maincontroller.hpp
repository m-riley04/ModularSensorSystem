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
	SourceController m_sourceController;
	PluginController m_pluginController;
	PresetsController m_presetsController;
	ProcessingController m_processingController;
	MountController m_mountController;
	SessionController m_sessionController;
	SettingsController m_settingsController;
};
