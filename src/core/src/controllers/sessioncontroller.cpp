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

	// Convert list of sources to elements
	// TODO: optimize this, make it more elegant, avoid copying, and move it elsewhere if possible
	QList<Element*> elements;
	QList<IRecordable*> recordableElements;
	for (auto& source : m_elementsController.sourceController().sources()) {
		if (!source->asPipelineElement()) continue; // Skip sources that can't be pipeline elements (vast majority should be able to)
		if (auto rec = source->asRecordable()) recordableElements.append(rec);
		elements.append(source);
	}

	// Also add mounts as GST elements (if they have bins)
	for (auto& mount : m_elementsController.mountController().mounts()) {
		if (!mount->asPipelineElement()) continue; // Skip mounts that can't be pipeline elements
		if (auto rec = mount->asRecordable()) recordableElements.append(rec);
		elements.append(mount);
	}

	// Finally, add processors as GST elements
	//for (auto& processor : m_elementsController.processingController().processors()) {
	//	if (!processor->asPipelineElement()) continue; // Skip processors that can't be pipeline elements
	//	if (auto rec = processor->asRecordable()) recordableElements.append(rec);
	//	elements.append(processor);
	//}

	m_pipeline.build(elements, recordableElements);
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
