#include "pipeline/sessionpipeline.hpp"

SessionPipeline::SessionPipeline(SessionProperties& properties, QObject* parent)
	: QObject(parent), m_pipeline(nullptr, &gst_object_unref), m_sessionProperties(&properties)
{
	if (!gst_is_initialized()) {
		gst_init(nullptr, nullptr);
	}
}

static gboolean pipeline_bus_call(GstBus* bus, GstMessage* msg, gpointer data)
{
	SessionPipeline* sessionPipeline = (SessionPipeline*)data;

	switch (GST_MESSAGE_TYPE(msg)) {

	case GST_MESSAGE_EOS:
		QMetaObject::invokeMethod(sessionPipeline, "onPipelineEos", Qt::QueuedConnection);
		break;
	case GST_MESSAGE_WARNING: {
		gchar* debug;
		GError* error;

		gst_message_parse_warning(msg, &error, &debug);
		g_free(debug);

		// Notify session pipeline of the warning/error
		QMetaObject::invokeMethod(sessionPipeline, "onPipelineError", Qt::QueuedConnection,
			Q_ARG(QString, QString::fromUtf8(error->message)));
		g_error_free(error);
		break;
	}
	case GST_MESSAGE_ERROR: {
		gchar* debug;
		GError* error;

		gst_message_parse_error(msg, &error, &debug);
		g_free(debug);

		// Notify session pipeline of the error
		QMetaObject::invokeMethod(sessionPipeline, "onPipelineError", Qt::QueuedConnection,
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

bool SessionPipeline::build(SessionProperties& properties, const QList<Source*>& sources, const QList<IRecordableSource*>& recordableSources)
{
	// Cleanly tear down any existing pipeline first
	close();

	// Set session properties
	m_sessionProperties = &properties;
	m_recordableSources = recordableSources;

	// Create the main pipeline
	m_pipeline.reset(GST_PIPELINE(gst_pipeline_new(MAIN_PIPELINE_NAME)));
	if (!m_pipeline) {
		qWarning() << "Failed to create GStreamer pipeline";
		return false;
	}

	// Add bus watch
	GstBus* bus;
	bus = gst_pipeline_get_bus(m_pipeline.get());
	m_pipelineBusWatchId = gst_bus_add_watch(bus, pipeline_bus_call, this);
	gst_object_unref(bus);

	// Iterate over all sources and add them
	for (auto& src : sources) {
		if (!createSourceElements(src)) {
			emit errorOccurred("Failed to create source elements for source '" + QString::fromStdString(src->name()) + "'");
			close();
			return false;
		}

		// Link start and stop hooks
		connect(this, &SessionPipeline::started, src, &Source::onSessionStart);
		connect(this, &SessionPipeline::stopped, src, &Source::onSessionStop);
	}

	if (!start()) {
		emit errorOccurred("Failed to start pipeline after building.");
		close();
		return false;
	}

	setState(State::STARTED);

	return true;
}

bool SessionPipeline::close()
{
	// Stop pipeline first
	if (!this->stop()) {
		qWarning() << "Failed to stop pipeline";
		return false;
	}

	this->cleanup();
	setState(State::STOPPED);

	return true;
}

bool SessionPipeline::start()
{
	if (!m_pipeline) {
		qWarning() << "Cannot start pipeline: pipeline is null";
		return false;
	}

	// Step through states to surface potential errors
	if (gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_READY) == GST_STATE_CHANGE_FAILURE) {
		qWarning() << "Failed to set pipeline to READY";
		close();
		return false;
	}
	if (gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_PAUSED) == GST_STATE_CHANGE_FAILURE) {
		qWarning() << "Failed to set pipeline to PAUSED";
		close();
		return false;
	}
	if (gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
		qWarning() << "Failed to set pipeline to PLAYING";
		close();
		return false;
	}

	return true;
}

bool SessionPipeline::stop()
{
	if (!m_pipeline) {
		return true;
	}

	if (gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_NULL) == GST_STATE_CHANGE_FAILURE) {
		qWarning() << "Failed to set pipeline to NULL";
		return false;
	}

	return true;
}

bool SessionPipeline::cleanup()
{
	// Pipeline cleanup
	m_pipeline.reset(nullptr);

	// bus watch cleanup
	g_source_remove(m_pipelineBusWatchId);
	m_pipelineBusWatchId = 0;

	// GST elements ptrs lists cleanup
	m_sourceBins.clear();
	m_previewBins.clear();
	m_recordBins.clear();
	m_recordableSourceBins.clear();

	// Sources ptrs list cleanup
	m_recordableSources.clear();

	// Session properties ptr cleanup
	m_sessionProperties = nullptr;
	return true;
}

void SessionPipeline::startRecording()
{
	m_lastRecordingTimestamp = generateTimestampNs();

	if (!openRecordingValves(m_recordableSources)) {
		qWarning() << "Failed to open recording valves during startRecording.";
		return;
	}

	setState(State::RECORDING);
}

void SessionPipeline::stopRecording()
{
	if (!closeRecordingValves(m_recordableSources)) {
		qWarning() << "Failed to close recording valves during stopRecording.";
		return;
	}

	setState(State::STARTED);
}

bool SessionPipeline::createSourceElements(Source* source)
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

bool SessionPipeline::createAndLinkPreviewBin(Source* src, GstElement* tee)
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

bool SessionPipeline::createAndLinkRecordBin(Source* src, GstElement* tee)
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

bool SessionPipeline::openRecordingValves(QList<IRecordableSource*>& sources)
{
	// Iterate over all sources and open their valves
	for (auto& src : sources) {
		if (!openRecordingValveForSource(src)) {
			qWarning() << "Failed to open recording valve for source";
		}
	}

	return true;
}

bool SessionPipeline::closeRecordingValves(QList<IRecordableSource*>& sources)
{
	// Iterate over all sources and close their valves
	for (auto& src : sources) {
		if (!closeRecordingValveForSource(src)) {
			qWarning() << "Failed to close recording valve for source";
		}
	}

	return true;
}

bool SessionPipeline::openRecordingValveForSource(IRecordableSource* src)
{
	if (!src) {
		qWarning() << "Cannot open recording valve for source: source is not recordable";
		return false;
	}

	src->startRecording();
	return true;
}

bool SessionPipeline::closeRecordingValveForSource(IRecordableSource* src)
{
	if (!src) {
		qWarning() << "Cannot close recording valve for source: source is not recordable";
		return false;
	}

	src->stopRecording();
	return true;
}

void SessionPipeline::setState(State newState)
{
	if (m_state == newState) {
		return;
	}

	switch (newState) {
	case State::STARTED:
		if (m_state == State::RECORDING) { // If we are transitioning from recording to started, emit recording stopped
			emit recordingStopped();
			break;
		}

		emit started();
		break;
	case State::STOPPED:
		emit stopped();
		break;
	case State::RECORDING:
		emit recordingStarted();
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