#include "controllers/sessioncontroller.hpp"

SessionController::SessionController(SettingsController& settingsController, SourceController& sourceController, ProcessingController& processingController,
	MountController& mountController, QObject* parent)
	: QObject(parent)
	, m_settingsController(settingsController)
	, m_pipeline(SessionPipeline(settingsController.sessionSettings(), this)), m_sourceController(sourceController), m_processingController(processingController),
	m_mountController(mountController)
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
	m_pipeline.build(m_sourceController.sources(), m_sourceController.recordableSources());
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

void SessionController::clearRecordings()
{
	QDir outputDir = m_settingsController.sessionSettings().outputDirectory;
	if (!outputDir.exists()) {
		return; // Nothing to clear
	}

	// TODO: make file extensions configurable
	QString sessionRecordingPrefix = m_settingsController.sessionSettings().outputPrefix;
	QStringList recordingDirs = outputDir.entryList(QStringList() << sessionRecordingPrefix << "*", QDir::Dirs);
	for (const QString& dirName : recordingDirs) {
		// Skip . and ..
		if (dirName == "." || dirName == "..") continue;
		QDir dir(outputDir.absoluteFilePath(dirName));
		if (!dir.removeRecursively()) {
			emit errorOccurred(QString("Failed to remove recording directory: %1").arg(dir.absolutePath()));
		}
	}
}

const QList<const Source*> SessionController::getSourcesByMount(QUuid mountId) const
{
	QList<const Source*> sources;
	const auto sourceIds = m_mountToSources.value(mountId);
	for (auto& id : sourceIds) {
		const Source* source = m_sourceController.byId(id);
		sources.push_back(source);
	}
	return sources;
}

const QList<const Processor*> SessionController::getProcessorsBySource(QUuid sourceId) const
{
	QList<const Processor*> processors;
	const auto processorIds = m_sourceToProcessors.value(sourceId);
	for (auto& id : processorIds) {
		const Processor* source = m_processingController.byId(id);
		processors.push_back(source);
	}
	return processors;
}