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
	delete m_sessionProperties;
}

/**
 * The callback function for GStreamer bus messages for the session pipeline.
 * Logic should not go here. Instead, it should be forwarded as Qt signals to the SessionController.
 * @param bus 
 * @param msg 
 * @param data 
 * @return 
 */
static gboolean session_bus_call(GstBus* bus, GstMessage* msg, gpointer data)
{
	SessionController* sessionController = (SessionController*)data;

	switch (GST_MESSAGE_TYPE(msg)) {

	case GST_MESSAGE_EOS:
		sessionController->pipelineEosReached();
		break;
	case GST_MESSAGE_WARNING: {
		gchar* debug;
		GError* error;

		gst_message_parse_warning(msg, &error, &debug);
		g_free(debug);

		// Notify session controller of the warning/error
		emit sessionController->errorOccurred(QString::fromUtf8(error->message));
		g_error_free(error);
		break;
	}
	case GST_MESSAGE_ERROR: {
		gchar* debug;
		GError* error;

		gst_message_parse_error(msg, &error, &debug);
		g_free(debug);

		// Notify session controller of the error
		emit sessionController->errorOccurred(QString::fromUtf8(error->message));
		g_error_free(error);

		break;
	}
	case GST_MESSAGE_UNKNOWN:
		g_print("Received unknown message.\n");
		break;
	default:
		break;
	}

	return TRUE;
}

void SessionController::buildPipeline()
{
	// Cleanly tear down any existing pipeline first
	closePipeline();

	// Create the main pipeline
	m_pipeline.reset(GST_PIPELINE(gst_pipeline_new(MAIN_PIPELINE_NAME)));
	if (!m_pipeline) {
		qWarning() << "Failed to create GStreamer pipeline";
		return;
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
		createSourceElements(src);

		// Link start and stop hooks
		connect(this, &SessionController::sessionStarted, src, &Source::onSessionStart);
		connect(this, &SessionController::sessionStopped, src, &Source::onSessionStop);
	}
	
	// Step through states to surface problems earlier
	if (gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_READY) == GST_STATE_CHANGE_FAILURE) {
		qWarning() << "Failed to set pipeline to READY";
		closePipeline();
		return;
	}
	if (gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_PAUSED) == GST_STATE_CHANGE_FAILURE) {
		qWarning() << "Failed to set pipeline to PAUSED";
		closePipeline();
		return;
	}
	if (gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
		qWarning() << "Failed to set pipeline to PLAYING";
		closePipeline();
		return;
	}

	emit sessionStarted();
}

void SessionController::closePipeline()
{
	if (!m_pipeline) return;

	gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_NULL);

	emit sessionStopped();

	// Should go after emit to allow sources to clean up
	// TODO: is this fine? I feel like it should be before the emit...
	m_pipeline.reset(nullptr);

	// Reset values
	g_source_remove(m_pipelineBusWatchId);
	m_pipelineBusWatchId = 0;
	m_sourceBins.clear();
	m_previewBins.clear();
	m_recordBins.clear();
	m_recordableSources.clear();
}

gboolean SessionController::createSourceElements(Source* source)
{
	// Check source
	if (!source) {
		qWarning() << "Cannot create source elements: source is null";
		return FALSE;
	}

	// Initialize src bin
	GstElement* srcBin = source->srcBin();

	// Check src bin
	if (!srcBin) {
		qWarning() << "Video source has no Gst bin";
		return FALSE;
	}

	// Add source bin to pipeline
	if (!gst_bin_add(GST_BIN(m_pipeline.get()), srcBin)) {
		qWarning() << "Failed to add source bin to pipeline.";
		return FALSE;
	}

	// Create a tee element to split the source output
	std::string teeName = "tee_" + boost::uuids::to_string(source->uuid());
	GstElement* tee = gst_element_factory_make("tee", teeName.c_str());
	if (!tee) {
		qWarning() << "Failed to create tee element for source:"
			<< QString::fromStdString(source->name());
		return FALSE;
	}

	// Add tee to pipeline
	if (!gst_bin_add(GST_BIN(m_pipeline.get()), tee)) {
		qWarning() << "Failed to add tee element to pipeline for source:"
			<< QString::fromStdString(source->name());
		return FALSE;
	}

	// Link source bin to tee
	if (!gst_element_link(srcBin, tee)) {
		qWarning() << "Failed to link source bin to tee element for source:"
			<< QString::fromStdString(source->name());
		gst_bin_remove(GST_BIN(m_pipeline.get()), tee);
		return FALSE;
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

	return TRUE;
}

gboolean SessionController::createAndLinkPreviewBin(Source* src, GstElement* tee)
{
	if (!src) {
		qWarning() << "Cannot create and link the source and preview bins: source is null";
		return FALSE;
	}

	IPreviewableSource* prevSrc = src->asPreviewable();

	if (!prevSrc) {
		qWarning() << "Cannot create and link the source and preview bins for '" << src->displayName() << "': source is not previewable";
		return FALSE;
	}

	// Init elemets
	guintptr windowId = static_cast<guintptr>(prevSrc->windowId());
	GstElement* sink = prevSrc->previewSinkBin();

	// TODO/CONSIDER: similar to recording, maybe include a valve mechanism to enable/disable previewing?

	// Check validity of each
	if (!sink) {
		qWarning() << "Failed to create custom sink element for '" << src->displayName() << "'; creating default sink";
		sink = createDefaultSink(src->type(), windowId, prevSrc->previewSinkElementName().c_str());
	}

	// Add preview element(s) to pipeline
	if (!gst_bin_add(GST_BIN(m_pipeline.get()), sink)) {
		qWarning() << "Failed to add preview sink for '" << src->displayName() << "' to pipeline.";
		return FALSE;
	}

	// Link source bin to sink
	if (!gst_element_link(tee, sink)) {
		qWarning() << "Failed to link source bin to preview sink for '" << src->displayName() << "'.";
		gst_bin_remove(GST_BIN(m_pipeline.get()), sink);
		return FALSE;
	}

	// If we succeed all paths above, add to our tracking list
	m_previewBins.append(sink);

	return TRUE;
}

gboolean SessionController::createAndLinkRecordBin(Source* src, GstElement* tee)
{
	if (!src) {
		qWarning() << "Cannot create and link the source and recording bins: source is null";
		return FALSE;
	}

	IRecordableSource* recSrc = src->asRecordable();

	if (!recSrc) {
		qWarning() << "Cannot create and link the source and recording bins for '" << src->displayName() << "': source is not recordable";
		return FALSE;
	}

	// Init elemets
	GstElement* sink = recSrc->recorderSinkBin();

	// Check validity of sink
	if (!sink) {
		qWarning() << "Failed to create custom sink element for '" << src->displayName() << "'; creating default sink";
		return FALSE;
	}

	// Set sink's output directory and prefix from session properties
	const QString outputFilePath = generateSessionSourcePath(src, *m_sessionProperties, m_lastSessionTimestamp);
	if (outputFilePath.isEmpty()) {
		qWarning() << "Cannot set recording file path: output file path is empty for source:" << QString::fromStdString(src->name());
		return FALSE;
	}
	// Use UTF-8 when passing path strings into GStreamer properties
	const std::string utf8Path = outputFilePath.toUtf8().toStdString();

	if (!recSrc->setRecordingFilePath(utf8Path)) {
		qWarning() << "Failed to set recording file path for source '"
			<< QString::fromStdString(src->displayName())
			<< "' to '"
			<< outputFilePath
			<< "'";
		return FALSE;
	}

	// Add recorder bin to pipeline
	if (!gst_bin_add(GST_BIN(m_pipeline.get()), sink)) {
		qWarning() << "Failed to add recorder sink for '" << src->displayName() << "' to pipeline.";
		return FALSE;
	}

	// Tee branches must have queues; the recorder bin now begins with a queue
	if (!gst_element_link(tee, sink)) {
		qWarning() << "Failed to link source bin tee to recorder bin for '" << src->displayName() << "'.";
		gst_bin_remove(GST_BIN(m_pipeline.get()), sink);
		return FALSE;
	}

	// If we succeed all paths above, add to our tracking list
	m_recordBins.append(sink);

	return TRUE;
}

gboolean SessionController::openRecordingValves()
{
	// Iterate over all sources and open their valves
	for (auto& src : m_sourceController->sources()) {
		if (!src->asRecordable()) {
			continue; // Skip non-recordable sources
		}

		openRecordingValveForSource(src);
	}

	return TRUE;
}

gboolean SessionController::closeRecordingValves()
{
	// Iterate over all sources and close their valves
	for (auto& src : m_sourceController->sources()) {
		if (!src->asRecordable()) {
			continue; // Skip non-recordable sources
		}

		bool r = closeRecordingValveForSource(src);
		if (!r) {
			qWarning() << "Failed to close recording valve for source '"
				<< QString::fromStdString(src->displayName())
				<< "'";
		}
	}

	return TRUE;
}

gboolean SessionController::openRecordingValveForSource(Source* src)
{
	if (IRecordableSource* recSrc = src->asRecordable()) {
		recSrc->startRecording();
		return TRUE;
	}

	qWarning() << "Cannot open recording valve for source '"
		<< QString::fromStdString(src->displayName())
		<< "': source is not recordable";
	return FALSE;
}

gboolean SessionController::closeRecordingValveForSource(Source* src)
{
	if (IRecordableSource* recSrc = src->asRecordable()) {
		recSrc->stopRecording();
		return TRUE;
	}

	qWarning() << "Cannot close recording valve for source '"
		<< QString::fromStdString(src->displayName())
		<< "': source is not recordable";
	return FALSE;
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

