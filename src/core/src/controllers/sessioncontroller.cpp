#include "controllers/sessioncontroller.hpp"
#include <utils/safer_io_utils.hpp>

SessionController::SessionController(SettingsController& settingsController, ElementsController& ec, QObject* parent)
	: QObject(parent)
	, m_settingsController(settingsController)
	, m_elementsController(ec)
	, m_pipeline(settingsController.sessionSettings(), m_elementsController, this)
{
	qRegisterMetaType<AutomationEvent>("AutomationEvent");

	connect(&m_pipeline, &SessionPipeline::errorOccurred, this, &SessionController::errorOccurred);

	connect(&m_pipeline, &SessionPipeline::stateChanged, this, [this](SessionPipeline::State newState) {
		AutomationEvent ev;
		ev.type = "pipeline.stateChanged";
		ev.payload.insert("state", static_cast<int>(newState));
		ev.timestamp = m_lastSessionTimestamp;
		emit automationEvent(ev);
	});

	connect(&m_pipeline, &SessionPipeline::eosReached, this, [this]() {
		AutomationEvent ev;
		ev.type = "pipeline.eos";
		ev.timestamp = m_lastSessionTimestamp;
		emit automationEvent(ev);
	});

	connect(&m_pipeline, &SessionPipeline::errorOccurred, this, [this](const QString& errorMessage) {
		AutomationEvent ev;
		ev.type = "pipeline.error";
		ev.payload.insert("message", errorMessage);
		ev.timestamp = m_lastSessionTimestamp;
		emit automationEvent(ev);
	});

	connect(&m_pipeline, &SessionPipeline::recordingStarted, this, [this]() {
		AutomationEvent ev;
		ev.type = "pipeline.recordingStarted";
		ev.timestamp = m_lastSessionTimestamp;
		emit automationEvent(ev);
	});

	connect(&m_pipeline, &SessionPipeline::recordingStopped, this, [this]() {
		AutomationEvent ev;
		ev.type = "pipeline.recordingStopped";
		ev.timestamp = m_lastSessionTimestamp;
		emit automationEvent(ev);
	});
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
		return;
	}

	// Forward processor events into controller-level automation bus
	for (Processor* processor : m_elementsController.processingController().processors()) {
		if (!processor) continue;
		connect(processor, &Processor::automationEvent,
			this, &SessionController::automationEvent, Qt::QueuedConnection);
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
