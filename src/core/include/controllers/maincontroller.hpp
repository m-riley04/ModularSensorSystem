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
#include "controllers/loggingcontroller.hpp"
#include "controllers/elementscontroller.hpp"
#include "core_export.hpp"

class MSS_CORE_API MainController  : public QObject
{
	Q_OBJECT

public:
	MainController(QSettings& settings, QObject *parent);
	~MainController();

	MountController& mountController() { return m_mountController; }
	SourceController& sourceController() { return m_sourceController; }
	ProcessingController& processingController() { return m_processingController; }
	ElementsController& elementsController() { return m_elementsController; }
	SessionController& sessionController() { return m_sessionController; }
	PluginController& pluginController() { return m_pluginController; }
	PresetsController& presetsController() { return m_presetsController; }
	SettingsController& settingsController() { return m_settingsController; }
	LoggingController& loggingController() { return m_loggingController; }

	/**
	 * @brief The main shutdown procedure to clean up resources before application exit.
	 */
	void shutdown();

private:
	SettingsController m_settingsController;
	LoggingController m_loggingController;
	SourceController m_sourceController;
	ProcessingController m_processingController;
	MountController m_mountController;
	ElementsController m_elementsController;
	PluginController m_pluginController;
	PresetsController m_presetsController;
	SessionController m_sessionController;
};
