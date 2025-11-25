#include "controllers/sessioncontroller.hpp"

SessionController::SessionController(SourceController* sourceController, ProcessingController* processingController, 
	MountController* mountController, QObject* parent)
	: BackendControllerBase("SessionController", parent), m_pipeline(new SessionPipeline(this)),
	m_sourceController(sourceController), m_processingController(processingController), 
	m_mountController(mountController), m_sessionProperties(new SessionProperties()),
	m_pipelineThread(new QThread(this))
{

	// Create sessions directory if it doesn't exist
	QDir sessionsDir = QDir(QCoreApplication::applicationDirPath() + DEFAULT_SESSIONS_DIRECTORY);
	if (!sessionsDir.exists()) {
		if (!QDir().mkpath(sessionsDir.absolutePath())) {
			qWarning() << "Failed to create sessions directory:" << sessionsDir.absolutePath() << "\nUsing application directory instead.";
			sessionsDir = QDir(QCoreApplication::applicationDirPath());
		}
	}

	// Connect signals for error handling
	connect(m_pipeline.get(), &SessionPipeline::errorOccurred, this, &SessionController::errorOccurred);
	
	// TODO: in the future, load the session properties from a saved state
	// For right now, just initialize with defaults
	*m_sessionProperties = {
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

	// Move pipeline to its own thread
	m_pipeline->moveToThread(m_pipelineThread);
}

SessionController::~SessionController()
{
	m_pipeline->close();
	m_pipelineThread->quit();
}

void SessionController::setSessionProperties(SessionProperties properties)
{
	*m_sessionProperties = properties;
	m_pipeline->setSessionProperties(m_sessionProperties.get());

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
	m_pipeline->setSessionTimestamp(m_lastSessionTimestamp);
	m_pipeline->build(m_sessionProperties.get(), m_sourceController->sources(), m_sourceController->recordableSources());
}

void SessionController::stopSession()
{
	m_pipeline->close();
}

void SessionController::startRecording()
{
	m_pipeline->startRecording();
}

void SessionController::stopRecording()
{
	m_pipeline->stopRecording();
}

QList<const Source*> SessionController::getSourcesByMount(QUuid mountId) const
{
	QList<const Source*> sources;
	const auto sourceIds = m_mountToSources.value(mountId);
	for (auto& id : sourceIds) {
		const Source* source = m_sourceController->byId(id);
		sources.push_back(source);
	}
	return sources;
}

QList<const Processor*> SessionController::getProcessorsBySource(QUuid sourceId) const
{
	QList<const Processor*> processors;
	const auto processorIds = m_sourceToProcessors.value(sourceId);
	for (auto& id : processorIds) {
		const Processor* source = m_processingController->byId(id);
		processors.push_back(source);
	}
	return processors;
}