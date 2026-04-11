#include "controllers/sessioncontroller.hpp"
#include <utils/safer_io_utils.hpp>
#include <models/rule_models.hpp>

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
		ev.type = AutomationEventStrings::PipelineStateChanged;
		ev.payload.insert("state", static_cast<int>(newState));
		ev.timestamp = m_lastSessionTimestamp;
		emit automationEvent(ev);
	});

	connect(&m_pipeline, &SessionPipeline::eosReached, this, [this]() {
		AutomationEvent ev;
		ev.type = AutomationEventStrings::PipelineEos;
		ev.timestamp = m_lastSessionTimestamp;
		emit automationEvent(ev);
	});

	connect(&m_pipeline, &SessionPipeline::errorOccurred, this, [this](const QString& errorMessage) {
		AutomationEvent ev;
		ev.type = AutomationEventStrings::PipelineError;
		ev.payload.insert("message", errorMessage);
		ev.timestamp = m_lastSessionTimestamp;
		emit automationEvent(ev);
	});

	connect(&m_pipeline, &SessionPipeline::recordingStarted, this, [this]() {
		AutomationEvent ev;
		ev.type = AutomationEventStrings::RecordingStarted;
		ev.timestamp = m_lastSessionTimestamp;
		emit automationEvent(ev);
	});

	connect(&m_pipeline, &SessionPipeline::recordingStopped, this, [this]() {
		AutomationEvent ev;
		ev.type = AutomationEventStrings::RecordingStopped;
		ev.timestamp = m_lastSessionTimestamp;
		emit automationEvent(ev);
	});

	// Wire future processors added mid-session
	connect(&m_elementsController.processingController(), &ProcessingController::processorAdded,
		this, [this](Processor* processor) {
			if (!processor) return;
			connectProcessor(processor);
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
		connectProcessor(processor);
	}

	// Emit session-started automation event
	AutomationEvent ev;
	ev.type = AutomationEventStrings::SessionStarted;
	ev.timestamp = m_lastSessionTimestamp;
	emit automationEvent(ev);
}

void SessionController::stopSession()
{
	LoggingController::info("Stopping session...");
	if (!m_pipeline.close()) {
		LoggingController::critical("Failed to stop session.");
	}

	// Emit session-stopped automation event
	AutomationEvent ev;
	ev.type = AutomationEventStrings::SessionStopped;
	ev.timestamp = m_lastSessionTimestamp;
	emit automationEvent(ev);
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

	// Emit processing-started automation event
	AutomationEvent ev;
	ev.type = AutomationEventStrings::ProcessingStarted;
	ev.timestamp = m_lastSessionTimestamp;
	emit automationEvent(ev);
}

void SessionController::stopProcessing()
{
	LoggingController::info("Stopping processing...");
	m_pipeline.stopProcessing();

	// Emit processing-stopped automation event
	AutomationEvent ev;
	ev.type = AutomationEventStrings::ProcessingStopped;
	ev.timestamp = m_lastSessionTimestamp;
	emit automationEvent(ev);
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

void SessionController::connectProcessor(Processor* processor)
{
	// Forward raw automation events from the processor
	connect(processor, &Processor::automationEvent,
		this, &SessionController::automationEvent, Qt::QueuedConnection);

	// Bridge objectsDetected into the automation event bus
	connect(processor, &Processor::objectsDetected,
		this, [this, processor](const std::vector<DetectionInfo>& detections) {
			if (detections.empty()) return;

			AutomationEvent ev;
			ev.type = AutomationEventStrings::ProcessorObjectDetected;
			ev.elementId = boostUuidToQUuid(processor->uuid());
			ev.timestamp = m_lastSessionTimestamp;

			// Summary
			ev.payload.insert("count", static_cast<int>(detections.size()));

			// Top-confidence detection details
			const auto& top = detections.front();
			ev.payload.insert("label", QString::fromUtf8(top.label, static_cast<qsizetype>(top.labelLength)));
			ev.payload.insert("confidence", static_cast<double>(top.confidence));
			ev.payload.insert("x", top.x);
			ev.payload.insert("y", top.y);
			ev.payload.insert("width", top.width);
			ev.payload.insert("height", top.height);

			// All detected labels for condition matching
			QVariantList labels;
			labels.reserve(static_cast<qsizetype>(detections.size()));
			for (const auto& d : detections) {
				labels.append(QString::fromUtf8(d.label, static_cast<qsizetype>(d.labelLength)));
			}
			ev.payload.insert("labels", labels);

			emit automationEvent(ev);
	}, Qt::QueuedConnection);
}
