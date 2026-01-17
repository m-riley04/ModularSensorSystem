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
	m_pipeline.close();
}

void SessionController::restartSession()
{
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
	m_pipeline.build(m_elementsController.elements());
}

void SessionController::stopSession()
{
	m_pipeline.close();
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

	safeDeleteDirectoryContents(
		m_settingsController.sessionSettings().outputDirectory
		, QStringList() << sessionRecordingPrefix << "*"
		, QDir::Dirs);
}
