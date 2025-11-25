#include "controllers/sessioncontroller.hpp"

SessionController::SessionController(SourceController* sourceController, ProcessingController* processingController, 
	MountController* mountController, QObject* parent)
	: BackendControllerBase("SessionController", parent), m_pipeline(nullptr, &gst_object_unref), 
	m_sourceController(sourceController), m_processingController(processingController), 
	m_mountController(mountController), m_sessionProperties(new SessionProperties())
{
	if (!gst_is_initialized()) {
		gst_init(nullptr, nullptr);
	}

	// Create sessions directory if it doesn't exist
	QDir sessionsDir = QDir(QCoreApplication::applicationDirPath() + DEFAULT_SESSIONS_DIRECTORY);
	if (!sessionsDir.exists()) {
		if (!QDir().mkpath(sessionsDir.absolutePath())) {
			qWarning() << "Failed to create sessions directory:" << sessionsDir.absolutePath() << "\nUsing application directory instead.";
			sessionsDir = QDir(QCoreApplication::applicationDirPath());
		}
	}

	// Connect internal signals
	connect(this, &SessionController::pipelineEosReached, [this]() {
		// Check if we are stopping the session or recording
		this->stopSession();
		});

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
}

SessionController::~SessionController()
{
	closePipeline();
}

/**
 * The callback function for GStreamer bus messages for the session pipeline.
 * Logic should not go here. Instead, it should be forwarded as Qt signals to the SessionController.
 * @return 
 */
static gboolean session_bus_call(GstBus* bus, GstMessage* msg, gpointer data)
{
	SessionController* sessionController = (SessionController*)data;

	switch (GST_MESSAGE_TYPE(msg)) {

	case GST_MESSAGE_EOS:
		QMetaObject::invokeMethod(sessionController, "pipelineEosReached", Qt::QueuedConnection);
		break;
	case GST_MESSAGE_WARNING: {
		gchar* debug;
		GError* error;

		gst_message_parse_warning(msg, &error, &debug);
		g_free(debug);

		// Notify session controller of the warning/error
		QMetaObject::invokeMethod(sessionController, "setPipelineError", Qt::QueuedConnection,
			Q_ARG(QString, QString::fromUtf8(error->message)));
		g_error_free(error);
		break;
	}
	case GST_MESSAGE_ERROR: {
		gchar* debug;
		GError* error;

		gst_message_parse_error(msg, &error, &debug);
		g_free(debug);

		// Notify session controller of the error
		QMetaObject::invokeMethod(sessionController, "setPipelineError", Qt::QueuedConnection,
			Q_ARG(QString, QString::fromUtf8(error->message)));
		g_error_free(error);

		break;
	}
	case GST_MESSAGE_UNKNOWN:
		g_print("Received unknown message.\n");
		break;
	default:
		break;
	}

	return true;
}

bool SessionController::buildPipeline()
{
	// Cleanly tear down any existing pipeline first
	closePipeline();

	// Create the main pipeline
	m_pipeline.reset(GST_PIPELINE(gst_pipeline_new(MAIN_PIPELINE_NAME)));
	if (!m_pipeline) {
		qWarning() << "Failed to create GStreamer pipeline";
		return false;
	}

	// Generate a new session timestamp
	m_lastSessionTimestamp = generateTimestampNs();

	// Add bus watch
	GstBus* bus;
	bus = gst_pipeline_get_bus(m_pipeline.get());
	m_pipelineBusWatchId = gst_bus_add_watch(bus, session_bus_call, this);
	gst_object_unref(bus);

	// Iterate over all sources and add them
	for (auto& src : m_sourceController->sources()) {
		if (!createSourceElements(src)) {
			setPipelineError("Failed to create source elements for source '" + QString::fromStdString(src->name()) + "'");
			closePipeline();
			return false;
		}

		// Link start and stop hooks
		connect(this, &SessionController::sessionStarted, src, &Source::onSessionStart);
		connect(this, &SessionController::sessionStopped, src, &Source::onSessionStop);
	}

	return true;
}

bool SessionController::closePipeline()
{
	if (this->stopPipeline()) {
		qWarning() << "Failed to stop pipeline";
		return false;
	}

	// Reset values
	m_pipeline.reset(nullptr);
	g_source_remove(m_pipelineBusWatchId);
	m_pipelineBusWatchId = 0;
	m_sourceBins.clear();
	m_previewBins.clear();
	m_recordBins.clear();
	m_recordableSources.clear();

	return true;
}

bool SessionController::startPipeline()
{
	if (!m_pipeline) {
		qWarning() << "Cannot start pipeline: pipeline is null";
		return false;
	}

	// Step through states to surface potential errors
	if (gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_READY) == GST_STATE_CHANGE_FAILURE) {
		qWarning() << "Failed to set pipeline to READY";
		closePipeline();
		return false;
	}
	if (gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_PAUSED) == GST_STATE_CHANGE_FAILURE) {
		qWarning() << "Failed to set pipeline to PAUSED";
		closePipeline();
		return false;
	}
	if (gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
		qWarning() << "Failed to set pipeline to PLAYING";
		closePipeline();
		return false;
	}

	return true;
}

bool SessionController::stopPipeline()
{
	if (gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_NULL) == GST_STATE_CHANGE_FAILURE) {
		qWarning() << "Failed to set pipeline to NULL";
		return false;
	}

	return true;
}

bool SessionController::createSourceElements(Source* source)
{
	// Check source
	if (!source) {
		qWarning() << "Cannot create source elements: source is null";
		return false;
	}

	// Initialize src bin
	GstElement* srcBin = source->srcBin();

	// Check src bin
	if (!srcBin) {
		qWarning() << "Video source has no Gst bin";
		return false;
	}

	// Add source bin to pipeline
	if (!gst_bin_add(GST_BIN(m_pipeline.get()), srcBin)) {
		qWarning() << "Failed to add source bin to pipeline.";
		return false;
	}

	// Create a tee element to split the source output
	std::string teeName = "tee_" + boost::uuids::to_string(source->uuid());
	GstElement* tee = gst_element_factory_make("tee", teeName.c_str());
	if (!tee) {
		qWarning() << "Failed to create tee element for source:"
			<< QString::fromStdString(source->name());
		return false;
	}

	// Add tee to pipeline
	if (!gst_bin_add(GST_BIN(m_pipeline.get()), tee)) {
		qWarning() << "Failed to add tee element to pipeline for source:"
			<< QString::fromStdString(source->name());
		return false;
	}

	// Link source bin to tee
	if (!gst_element_link(srcBin, tee)) {
		qWarning() << "Failed to link source bin to tee element for source:"
			<< QString::fromStdString(source->name());
		gst_bin_remove(GST_BIN(m_pipeline.get()), tee);
		return false;
	}

	// If we succeed all paths above, add to our tracking list
	m_sourceBins.append(srcBin);

	// Attempt to add/link preview
	if (source->asPreviewable() != nullptr) {
		if (!createAndLinkPreviewBin(source, tee)) {
			qWarning() << "Failed to create and link preview bin for source:"
				<< QString::fromStdString(source->name());
		}
	}

	// Attempt to add/link recording bin
	if (source->asRecordable() != nullptr) {
		if (!createAndLinkRecordBin(source, tee)) {
			qWarning() << "Failed to create and link recording bin for source:"
				<< QString::fromStdString(source->name());
		}
	}

	return true;
}

bool SessionController::createAndLinkPreviewBin(Source* src, GstElement* tee)
{
	if (!src) {
		qWarning() << "Cannot create and link the source and preview bins: source is null";
		return false;
	}

	IPreviewableSource* prevSrc = src->asPreviewable();

	if (!prevSrc) {
		qWarning() << "Cannot create and link the source and preview bins for '" << src->displayName() << "': source is not previewable";
		return false;
	}

	// Init elemets
	guintptr windowId = static_cast<guintptr>(prevSrc->windowId());
	GstElement* sink = prevSrc->previewSinkBin();

	// TODO/CONSIDER: similar to recording, maybe include a valve mechanism to enable/disable previewing?

	// Check validity of each
	if (!sink) {
		qWarning() << "Failed to create custom sink element for '" << src->displayName() << "'; creating default sink";
		sink = createDefaultPreviewSink(src->type(), windowId, prevSrc->previewSinkElementName().c_str());
	}

	// Add preview element(s) to pipeline
	if (!gst_bin_add(GST_BIN(m_pipeline.get()), sink)) {
		qWarning() << "Failed to add preview sink for '" << src->displayName() << "' to pipeline.";
		return false;
	}

	// Link source bin to sink
	if (!gst_element_link(tee, sink)) {
		qWarning() << "Failed to link source bin to preview sink for '" << src->displayName() << "'.";
		gst_bin_remove(GST_BIN(m_pipeline.get()), sink);
		return false;
	}

	// If we succeed all paths above, add to our tracking list
	m_previewBins.append(sink);

	return true;
}

bool SessionController::createAndLinkRecordBin(Source* src, GstElement* tee)
{
	if (!src) {
		qWarning() << "Cannot create and link the source and recording bins: source is null";
		return false;
	}

	IRecordableSource* recSrc = src->asRecordable();

	if (!recSrc) {
		qWarning() << "Cannot create and link the source and recording bins for '" << src->displayName() << "': source is not recordable";
		return false;
	}

	// Init elemets
	GstElement* sink = recSrc->recorderSinkBin();

	// Check validity of sink
	if (!sink) {
		qWarning() << "Failed to create custom sink element for '" << src->displayName() << "'; creating default sink";
		return false;
	}

	// Set sink's output directory and prefix from session properties
	const QString outputFilePath = generateSessionSourcePath(src, *m_sessionProperties, m_lastSessionTimestamp);
	if (outputFilePath.isEmpty()) {
		qWarning() << "Cannot set recording file path: output file path is empty for source:" << QString::fromStdString(src->name());
		return false;
	}
	// Use UTF-8 when passing path strings into GStreamer properties
	const std::string utf8Path = outputFilePath.toUtf8().toStdString();

	if (!recSrc->setRecordingFilePath(utf8Path)) {
		qWarning() << "Failed to set recording file path for source '"
			<< QString::fromStdString(src->displayName())
			<< "' to '"
			<< outputFilePath
			<< "'";
		return false;
	}

	// Add recorder bin to pipeline
	if (!gst_bin_add(GST_BIN(m_pipeline.get()), sink)) {
		qWarning() << "Failed to add recorder sink for '" << src->displayName() << "' to pipeline.";
		return false;
	}

	// Tee branches must have queues; the recorder bin now begins with a queue
	if (!gst_element_link(tee, sink)) {
		qWarning() << "Failed to link source bin tee to recorder bin for '" << src->displayName() << "'.";
		gst_bin_remove(GST_BIN(m_pipeline.get()), sink);
		return false;
	}

	// If we succeed all paths above, add to our tracking list
	m_recordBins.append(sink);

	return true;
}

bool SessionController::openRecordingValves()
{
	// Iterate over all sources and open their valves
	for (auto& src : m_sourceController->recordableSources()) {
		openRecordingValveForSource(src);
	}

	return true;
}

bool SessionController::closeRecordingValves()
{
	// Iterate over all sources and close their valves
	for (auto& src : m_sourceController->recordableSources()) {
		bool r = closeRecordingValveForSource(src);
		if (!r) qWarning() << "Failed to close recording valve for source";
	}

	return true;
}

bool SessionController::openRecordingValveForSource(IRecordableSource* src)
{
	if (!src) {
		qWarning() << "Cannot open recording valve for source: source is not recordable";
		return false;
	}

	src->startRecording();
	return true;
}

bool SessionController::closeRecordingValveForSource(IRecordableSource* src)
{
	if (src) {
		qWarning() << "Cannot close recording valve for source: source is not recordable";
		return false;
	}

	src->stopRecording();
	return true;
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

void SessionController::setSessionProperties(SessionProperties properties)
{
	*m_sessionProperties = properties;

	emit sessionPropertiesChanged(properties);
}

void SessionController::restartSession()
{
	closePipeline();
	buildPipeline();
	emit sessionRestarted();
}

void SessionController::setState(State newState)
{
	if (m_state == newState) {
		return;
	}

	switch (newState) {
	case State::STOPPING:
		emit sessionStopping();
		break;
	case State::STARTING:
		emit sessionStarting();
		break;
	case State::STARTED:
		if (m_state == State::STOPPING_RECORDING) { // If we are transitioning from stopping recording to started, emit recording stopped
			emit recordingStopped();
			break;
		}

		emit sessionStarted();
		break;
	case State::STOPPED:
		emit sessionStopped();
		break;
	case State::RECORDING:
		emit recordingStarted();
		break;
	case State::STOPPING_RECORDING:
		emit recordingStopping();
		break;
	case State::ERROR:
		// Handled elsewhere
		break;
	default:
		qWarning() << "Invalid session state transition attempted.";
		return;
	}

	m_state = newState;
	emit stateChanged(newState);
}

void SessionController::startSession()
{
	buildPipeline();
	setState(State::STARTED);
}

void SessionController::requestStopSession()
{
	setState(State::STOPPING);
	gst_element_send_event(GST_ELEMENT(m_pipeline.get()), gst_event_new_eos()); // This will close recording valves as well
}

void SessionController::stopSession()
{
	if (isRecording()) {
		requestStopRecording(); // TODO: right now, this is fine. But if async stopping is implemented, need to wait for that to finish first
	}

	closePipeline();
	setState(State::STOPPED);
}

void SessionController::startRecording()
{
	m_lastRecordingTimestamp = generateTimestampNs();

	if (!openRecordingValves()) {
		return;
	}

	setState(State::RECORDING);
}

void SessionController::stopRecording()
{
	if (!closeRecordingValves()) {
		qWarning() << "Failed to close recording valves during stopRecording.";
		return;
	}

	setState(State::STARTED);
}

void SessionController::requestStopRecording()
{
	setState(State::STOPPING_RECORDING);
	stopRecording(); // For now, just call stopRecording directly since I found a way to do it synchronously.
}

void SessionController::setPipelineError(const QString& errorMessage)
{
	qWarning() << "Pipeline error occurred:" << errorMessage;
	emit errorOccurred(errorMessage);
	// TODO/CONSIDER: set state to ERROR? Or maybe just leave it to the user to decide what to do next
	//requestStopSession(); // TODO/CONSIDER: decide whether to stop session on error
}

