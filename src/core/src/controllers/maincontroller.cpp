#include "controllers/maincontroller.hpp"


MainController::MainController(QSettings& settings, QObject *parent)
	: QObject(parent)
	, m_settingsController(SettingsController(settings, this))
	, m_sourceController(SourceController(this))
	, m_processingController(ProcessingController(this))
	, m_mountController(MountController(this))
	, m_pluginController(PluginController(m_settingsController, this))
	, m_presetsController(PresetsController(m_settingsController, this))
	, m_sessionController(SessionController(m_settingsController, m_sourceController, m_processingController, m_mountController, this))
	
{}

MainController::~MainController()
{}
