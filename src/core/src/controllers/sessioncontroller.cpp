#include "controllers/sessioncontroller.hpp"
#include <utils/safer_io_utils.hpp>

SessionController::SessionController(SettingsController& settingsController, ElementsController& ec, QObject* parent)
	: QObject(parent)
	, m_settingsController(settingsController)
	, m_elementsController(ec)
	, m_pipeline(settingsController.sessionSettings(), m_elementsController, this)
{
	// Connect signals for error handling
	connect(&m_pipeline, &SessionPipeline::errorOccurred, this, &SessionController::errorOccurred);
}

SessionController::~SessionController()
{
	LoggingController::info("Shutting down SessionController...");
	if (!m_pipeline.close()) {
		LoggingController::critical("Failed to close pipeline during shutdown");
	}
}

void SessionController::restartSession()
{
	LoggingController::info("Restarting session...");
	stopSession();
	startSession();
	emit sessionRestarted();
}

void SessionController::startSession()
{
	// Generate a new session timestamp
	m_lastSessionTimestamp = generateTimestampNs();
	m_pipeline.setSessionTimestamp(m_lastSessionTimestamp);

	// Build the pipeline
	if (!m_pipeline.build(m_elementsController.elements())) {
		LoggingController::critical("Failed to start session.");
	}
}

void SessionController::stopSession()
{
	LoggingController::info("Stopping session...");
	if (!m_pipeline.close()) {
		LoggingController::critical("Failed to stop session.");
	}
}

void SessionController::startRecording()
{
	LoggingController::info("Starting recording...");
	m_pipeline.startRecording();
}

void SessionController::stopRecording()
{
	LoggingController::info("Stopping recording...");
	m_pipeline.stopRecording();
}

void SessionController::startProcessing()
{
	LoggingController::info("Starting processing...");
	m_pipeline.startProcessing();
}

void SessionController::stopProcessing()
{
	LoggingController::info("Stopping processing...");
	m_pipeline.stopProcessing();
}

void SessionController::clearRecordings()
{
	QString sessionRecordingPrefix = m_settingsController.sessionSettings().outputPrefix;

	if (!safeDeleteDirectoryContents(
		m_settingsController.sessionSettings().outputDirectory
		, QStringList() << sessionRecordingPrefix << "*"
		, QDir::Dirs)) {
		LoggingController::critical("Failed to clear recordings.");
	}
}
