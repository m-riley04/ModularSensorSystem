#include "controllers/sessioncontroller.hpp"

static SessionProperties initSessionProps() {
	// Create sessions directory if it doesn't exist
	QDir sessionsDir = QDir(QCoreApplication::applicationDirPath() + DEFAULT_SESSIONS_DIRECTORY);
	if (!sessionsDir.exists()) {
		if (!QDir().mkpath(sessionsDir.absolutePath())) {
			qWarning() << "Failed to create sessions directory:" << sessionsDir.absolutePath() << "\nUsing application directory instead.";
			sessionsDir = QDir(QCoreApplication::applicationDirPath());
		}
	}

	return {
		.generalProperties = {},
		.clippingProperties = {
			.enabled = false,
		},
		.recordingProperties = {
			.outputDirectory = sessionsDir,
			.outputPrefix = DEFAULT_SESSION_PREFIX,
		},
		.processingProperties = {
			.enabled = false,
		},
	};
}

SessionController::SessionController(SourceController& sourceController, ProcessingController& processingController, 
	MountController& mountController, QObject* parent)
	: BackendControllerBase("SessionController", parent), m_sessionProperties(initSessionProps()),
	m_pipeline(SessionPipeline(m_sessionProperties, this)), m_sourceController(sourceController), m_processingController(processingController),
	m_mountController(mountController)
{
	// TODO: in the future, load the session properties from a saved state. For right now, they're default.

	// Connect signals for error handling
	connect(&m_pipeline, &SessionPipeline::errorOccurred, this, &SessionController::errorOccurred);
}

SessionController::~SessionController()
{
	m_pipeline.close();
}

void SessionController::setSessionProperties(const SessionProperties& properties)
{
	m_sessionProperties = properties;
	m_pipeline.setSessionProperties(m_sessionProperties);

	emit sessionPropertiesChanged(properties);
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
	m_pipeline.build(m_sessionProperties, m_sourceController.sources(), m_sourceController.recordableSources());
}

void SessionController::stopSession()
{
	m_pipeline.close();
}

void SessionController::startRecording()
{
	m_pipeline.startRecording();
}

void SessionController::stopRecording()
{
	m_pipeline.stopRecording();
}

QList<const Source*>& SessionController::getSourcesByMount(QUuid mountId) const
{
	QList<const Source*> sources;
	const auto sourceIds = m_mountToSources.value(mountId);
	for (auto& id : sourceIds) {
		const Source* source = m_sourceController.byId(id);
		sources.push_back(source);
	}
	return sources;
}

QList<const Processor*>& SessionController::getProcessorsBySource(QUuid sourceId) const
{
	QList<const Processor*> processors;
	const auto processorIds = m_sourceToProcessors.value(sourceId);
	for (auto& id : processorIds) {
		const Processor* source = m_processingController.byId(id);
		processors.push_back(source);
	}
	return processors;
}