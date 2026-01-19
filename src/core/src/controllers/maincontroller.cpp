#include "controllers/maincontroller.hpp"


MainController::MainController(QSettings& settings, QObject *parent)
	: QObject(parent)
	, m_settingsController(settings, this)
	, m_loggingController(m_settingsController, this)
	, m_sourceController(this)
	, m_processingController(this)
	, m_mountController(this)
	, m_elementsController(m_settingsController, m_mountController, m_sourceController, m_processingController, this)
	, m_pluginController(m_settingsController, this)
	, m_presetsController(m_settingsController, this)
	, m_sessionController(m_settingsController, m_elementsController, this)
	
{}

MainController::~MainController()
{
	shutdown();
}

void MainController::shutdown()
{
	LoggingController::info("=== Shutting down MainController ===");

	// Remove plugin-created QObjects from controllers to avoid dangling pointers
	m_mountController.clearMounts();
	m_sourceController.clearSources();
	m_processingController.clearProcessors();

	// Flush deferred deletes
	QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);

	// LASTLY, unload plugins
	m_pluginController.unloadPlugins();
}
