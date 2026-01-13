#include "pipeline/sessionpipeline.hpp"
#include <controllers/loggingcontroller.hpp>
#include <utils/gst_debug_utils.hpp>

SessionPipeline::SessionPipeline(SessionSettings& settings, ElementsController& ec, QObject* parent)
	: QObject(parent)
	, m_pipeline(nullptr, &gst_object_unref)
	, m_sessionSettings(settings)
	, m_elementsController(ec)
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

bool SessionPipeline::build(const QList<Element*>& elements, const QList<IRecordable*>& recordableElements)
{
	// Cleanly tear down any existing pipeline first
	close();

	// Create the main pipeline
	m_pipeline.reset(GST_PIPELINE(gst_pipeline_new(MAIN_PIPELINE_NAME)));
	if (!m_pipeline) {
		LoggingController::warning("Failed to create GStreamer pipeline");
		return false;
	}

	// Add bus watch
	GstBus* bus;
	bus = gst_pipeline_get_bus(m_pipeline.get());
	m_pipelineBusWatchId = gst_bus_add_watch(bus, pipeline_bus_call, this);
	gst_object_unref(bus);

	// Iterate over all sources and add them
	for (auto& element : elements) {
		if (!createSourceElements(element)) {
			emit errorOccurred("Failed to create source elements for element '" + QString::fromStdString(element->name()) + "'");
			close(); // TODO/CONSIDER: handle this more gracefully? And check if close succeeded?
			return false;
		}

		// Link start and stop hooks
		// TODO: fix connect syntax
		connect(this, &SessionPipeline::started, element, &Element::onSessionStart);
		connect(this, &SessionPipeline::stopped, element, &Element::onSessionStop);
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
		LoggingController::warning("Failed to stop pipeline");
		return false;
	}

	if (!this->cleanup()) {
		LoggingController::warning("Failed to clean up pipeline");
		return false;
	}

	setState(State::STOPPED);

	return true;
}

bool SessionPipeline::start()
{
	if (!m_pipeline) {
		LoggingController::warning("Cannot start pipeline: pipeline is null");
		return false;
	}

	// Set pipeline to playing
	if (gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
		LoggingController::warning("Failed to set pipeline to PLAYING");
		close();
		return false;
	}

	return true;
}

bool SessionPipeline::stop()
{
	if (!m_pipeline) return true;

	if (gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_NULL) == GST_STATE_CHANGE_FAILURE) {
		LoggingController::warning("Failed to set pipeline to NULL");
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

	// Branch lists cleanup
	m_previewBranches.clear();
	m_recordBranches.clear();
	m_processingBranches.clear();
	m_previewCompositors.clear();

	return true;
}

void SessionPipeline::startRecording()
{
	m_lastRecordingTimestamp = generateTimestampNs();

	// Iterate through all IRecordable elements and start recording
	for (IRecordable* recordableElement : this->m_elementsController.sourceController().recordableSources()) {
		if (!recordableElement) continue; // null check
		if (!recordableElement->startRecording()) {
			LoggingController::warning("Failed to start recording for element"); // todo: name element?
		}
	}

	setState(State::RECORDING);
}

void SessionPipeline::stopRecording()
{

	// Iterate through all IRecordable elements and start recording
	for (IRecordable* recordableElement : this->m_elementsController.sourceController().recordableSources()) {
		if (!recordableElement) continue; // null check
		if (!recordableElement->stopRecording()) {
			LoggingController::warning("Failed to stop recording for element"); // todo: name element?
		}
	}

	setState(State::STARTED);
}

void SessionPipeline::startProcessing()
{
	for (auto& processor : m_elementsController.processingController().processors()) {
		if (!processor) continue;
		if (!processor->startProcessing()) {
			LoggingController::warning("Failed to start processor: " + QString::fromStdString(processor->name()));
		}
	}
}

void SessionPipeline::stopProcessing()
{
	for (auto& processor : m_elementsController.processingController().processors()) {
		if (!processor) continue;
		if (!processor->stopProcessing()) {
			LoggingController::warning("Failed to stop processor: " + QString::fromStdString(processor->name()));
		}
	}
}

bool SessionPipeline::createSourceElements(Element* element)
{
	// Check source
	if (!element) {
		LoggingController::warning("Cannot create source elements: element is null");
		return false;
	}

	// Cast to pipeline element
	IPipelineElement* pipelineElem = element->asPipelineElement();
	if (!pipelineElem) {
		LoggingController::warning("Cannot create source elements for element '" + QString::fromStdString(element->displayName()) + "': element is not a pipeline element");
		return false;
	}

	// Initialize src bin
	GstElement* srcBin = pipelineElem->gstSrcBin();

	// Check src bin
	if (!srcBin) {
		LoggingController::warning("Source has no Gst bin");
		return false;
	}

	// Add source bin to pipeline
	if (!gst_bin_add(GST_BIN(m_pipeline.get()), srcBin)) {
		LoggingController::warning("Failed to add source bin to pipeline.");
		return false;
	}

	// If we succeed all paths above, add to our tracking list
	m_sourceBins.append(srcBin);

	// Create and link branches
	if (!createSourceBranches(element, srcBin)) {
		LoggingController::warning("Failed to create source branches for element:" + QString::fromStdString(element->name()));
		gst_bin_remove(GST_BIN(m_pipeline.get()), srcBin);
		return false;
	}

	return true;
}

bool SessionPipeline::createSourceBranches(Element* element, GstElement* srcBin)
{
	// Create a tee element to split the source output
	std::string teeName = "tee_" + boost::uuids::to_string(element->uuid());
	GstElement* tee = gst_element_factory_make("tee", teeName.c_str());
	if (!tee) {
		LoggingController::warning("Failed to create tee element for element:" + QString::fromStdString(element->name()));
		return false;
	}

	// Add tee to pipeline
	if (!gst_bin_add(GST_BIN(m_pipeline.get()), tee)) {
		LoggingController::warning("Failed to add tee element to pipeline for element:" + QString::fromStdString(element->name()));
		return false;
	}

	// Link source bin to tee
	if (!gst_element_link(srcBin, tee)) {
		LoggingController::warning("Failed to link source bin to tee element for element:" + QString::fromStdString(element->name()));
		gst_bin_remove(GST_BIN(m_pipeline.get()), tee);
		return false;
	}

	// Check if this source has processors attached
	QUuid sourceId = boostUuidToQUuid(element->uuid());
	QList<Processor*> processors = m_elementsController.processorsForSource(sourceId);
	bool hasProcessors = !processors.isEmpty();

	// Create processor branches first (they need to be in the pipeline before linking to preview)
	ProcessingBranch* processorBranch = nullptr;
	if (hasProcessors) {
		// For now, we only support one processor per source
		// TODO: Support chaining multiple processors
		Processor* processor = processors.first();
		processorBranch = createProcessorBranch(element, processor, tee);
		if (!processorBranch) {
			LoggingController::warning("Failed to create processor branch for element:" + QString::fromStdString(element->name()));
			// Continue without processor - preview will still work
			hasProcessors = false;
		}
	}

	// Attempt to add/link preview (with overlay support if processors exist)
	if (element->asPreviewable() != nullptr) {
		if (!createPreviewBranch(element, tee, processorBranch)) {
			LoggingController::warning("Failed to create preview branch for element:" + QString::fromStdString(element->name()));
			// TODO: should we pass through here?
		}
	}

	// Attempt to add/link recording bin
	if (element->asRecordable() != nullptr) {
		if (!createRecorderBranch(element, tee)) {
			LoggingController::warning("Failed to create recorder branch for element:" + QString::fromStdString(element->name()));
			// TODO: should we pass through here?
		}
	}

	return true;
}

bool SessionPipeline::createPreviewBranch(Element* element, GstElement* tee, ProcessingBranch* processorBranch)
{
	IPreviewable* prevElem = element->asPreviewable();
	if (!prevElem) {
		LoggingController::warning("Element is not previewable for element:" + QString::fromStdString(element->name()));
		return false;
	}

	// Get or create the preview branch with overlay support if processors are attached
	PreviewBranch* branch = prevElem->previewBranch();
	GstElement* branchBin = branch ? branch->bin() : nullptr;
	if (!branchBin) {
		LoggingController::warning("Failed to get preview branch bin for element:" + QString::fromStdString(element->name()));
		return false;
	}

	// Store preview branch (non-owning)
	m_previewBranches.append(branch);

	// Add preview branch to pipeline
	if (!gst_bin_add(GST_BIN(m_pipeline.get()), branchBin)) {
		LoggingController::warning("Failed to add preview sink for '" + QString::fromStdString(element->displayName()) + "' to pipeline.");
		return false;
	}

	// If we have a processor branch, create preview compositor links
	if (processorBranch) {
		m_previewCompositors.emplace_back(std::make_unique<PreviewCompositor>(m_pipeline.get(), tee, branch));
		if (!m_previewCompositors.back()->linkProcessingBranch(processorBranch)) {
			LoggingController::warning("Failed to link processing branch for element:" + QString::fromStdString(element->name()));
			return false;
		}
	}
	else if (!gst_element_link(tee, branchBin)) { // Otherwise, link tee directly to preview branch
		LoggingController::warning("Failed to link source bin to preview sink for '" + QString::fromStdString(element->displayName()) + "'.");
		gst_bin_remove(GST_BIN(m_pipeline.get()), branchBin);
		return false;
	}

	return true;
}

ProcessingBranch* SessionPipeline::createProcessorBranch(Element* sourceElement, Processor* processor, GstElement* tee)
{
	if (!processor) {
		LoggingController::warning("Cannot create processor branch: processor is null");
		return nullptr;
	}

	// Get the processor's pipeline element interface
	IPipelineElement* pipelineElem = processor->asPipelineElement();
	if (!pipelineElem) {
		LoggingController::warning("Processor '" + QString::fromStdString(processor->displayName()) + "' is not a pipeline element");
		return nullptr;
	}

	// Get the ProcessingBranch from the processor
	ProcessingBranch* processorBranch = pipelineElem->processingBranch();
	if (!processorBranch) {
		LoggingController::warning("Processor '" + QString::fromStdString(processor->displayName()) + "' does not expose a ProcessingBranch");
		return nullptr;
	}

	// Get the processor's bin
	GstElement* filterBin = processorBranch->bin();
	if (!filterBin) {
		LoggingController::warning("Failed to get filter bin for processor '" + QString::fromStdString(processor->displayName()) + "'");
		return nullptr;
	}

	// Add processor bin to pipeline
	if (!gst_bin_add(GST_BIN(m_pipeline.get()), filterBin)) {
		LoggingController::warning("Failed to add processor bin for '" + QString::fromStdString(processor->displayName()) + "' to pipeline.");
		return nullptr;
	}

	// Link tee to processor bin
	if (!gst_element_link(tee, filterBin)) {
		LoggingController::warning("Failed to link tee to processor bin for '" + QString::fromStdString(processor->displayName()) + "'.");
		gst_bin_remove(GST_BIN(m_pipeline.get()), filterBin);
		return nullptr;
	}

	// Store references
	m_processingBranches.append(processorBranch);

	LoggingController::debug("Created processor branch for '" + QString::fromStdString(processor->displayName()) + "'");

	return processorBranch;
}

bool SessionPipeline::createRecorderBranch(Element* element, GstElement* tee)
{
	if (!createAndLinkRecordBin(element, tee)) {
		LoggingController::warning("Failed to create and link recording bin for element:" + QString::fromStdString(element->name()));
		return false;
	}

	return true;
}

bool SessionPipeline::createAndLinkPreviewBin(Element* element, GstElement* tee)
{
	if (!element) {
		LoggingController::warning("Cannot create and link the source and preview bins: element is null");
		return false;
	}

	IPreviewable* prevSrc = element->asPreviewable();
	if (!prevSrc) {
		LoggingController::warning("Cannot create and link the source and preview bins for '" + QString::fromStdString(element->displayName()) + "': source is not previewable");
		return false;
	}

	// Init elemets
	guintptr windowId = static_cast<guintptr>(prevSrc->windowId());
	GstElement* sink = prevSrc->previewSinkBin();

	// TODO/CONSIDER: similar to recording, maybe include a valve mechanism to enable/disable previewing?

	// dynamic cast to source
	// TODO: this should be reworked to not assume element is a Source. Currently needed for "createDefaultPreviewSink" function
	Source* srcElem = dynamic_cast<Source*>(element);
	if (!srcElem) {
		LoggingController::warning("Cannot create and link the source and preview bins for '" + QString::fromStdString(element->displayName()) + "': element is not a source");
		return false;
	}

	// Check validity of each
	if (!sink) {
		LoggingController::warning("Failed to create custom sink element for '" + QString::fromStdString(element->displayName()) + "'; creating default sink");
		sink = createDefaultPreviewSink(srcElem->type(), windowId, prevSrc->previewSinkElementName().c_str());
	}

	// Add preview element(s) to pipeline
	if (!gst_bin_add(GST_BIN(m_pipeline.get()), sink)) {
		LoggingController::warning("Failed to add preview sink for '" + QString::fromStdString(element->displayName()) + "' to pipeline.");
		return false;
	}

	// Link source bin to sink
	if (!gst_element_link(tee, sink)) {
		LoggingController::warning("Failed to link source bin to preview sink for '" + QString::fromStdString(element->displayName()) + "'.");
		gst_bin_remove(GST_BIN(m_pipeline.get()), sink);
		return false;
	}

	return true;
}

bool SessionPipeline::createAndLinkRecordBin(Element* element, GstElement* tee)
{
	if (!element) {
		LoggingController::warning("Cannot create and link the source and recording bins: source is null");
		return false;
	}

	IRecordable* recSrc = element->asRecordable();
	if (!recSrc) {
		LoggingController::warning("Cannot create and link the source and recording bins for '" + QString::fromStdString(element->displayName()) + "': source is not recordable");
		return false;
	}

	// Init elemets
	GstElement* sink = recSrc->recorderSinkBin();

	// Check validity of sink
	if (!sink) {
		LoggingController::warning("Failed to create custom sink element for '" + QString::fromStdString(element->displayName()) + "'; creating default sink");
		return false;
	}

	// Set sink's output directory and prefix from session properties
	// TODO: rename away from "sources" and towards "elements"
	const QString outputFilePath = generateSessionSourcePath(element, m_sessionSettings, m_lastSessionTimestamp);
	if (outputFilePath.isEmpty()) {
		LoggingController::warning("Cannot set recording file path: output file path is empty for source:" + QString::fromStdString(element->name()));
		return false;
	}
	// Use UTF-8 when passing path strings into GStreamer properties
	const std::string utf8Path = outputFilePath.toUtf8().toStdString();

	if (!recSrc->setRecordingFilePath(utf8Path)) {
		LoggingController::warning("Cannot set recording file path for source '"
			+ QString::fromStdString(element->displayName())
			+ "' to '"
			+ outputFilePath
			+ "'");
		return false;
	}

	// Add recorder bin to pipeline
	if (!gst_bin_add(GST_BIN(m_pipeline.get()), sink)) {
		LoggingController::warning("Failed to add recorder sink for '" + QString::fromStdString(element->displayName()) + "' to pipeline.");
		return false;
	}

	// Tee branches must have queues; the recorder bin now begins with a queue
	if (!gst_element_link(tee, sink)) {
		LoggingController::warning("Failed to link source bin tee to recorder bin for '" + QString::fromStdString(element->displayName()) + "'.");
		gst_bin_remove(GST_BIN(m_pipeline.get()), sink);
		return false;
	}

	return true;
}

bool SessionPipeline::openRecordingValves(QList<IRecordable*>& elements)
{
	// Iterate over all sources and open their valves
	for (auto& element : elements) {
		if (!openRecordingValveForElement(element)) {
			LoggingController::warning("Failed to open recording valve for source");
		}
	}

	return true;
}

bool SessionPipeline::closeRecordingValves(QList<IRecordable*>& sources)
{
	// Iterate over all sources and close their valves
	for (auto& src : sources) {
		if (!closeRecordingValveForElement(src)) {
			LoggingController::warning("Failed to close recording valve for source");
		}
	}

	return true;
}

bool SessionPipeline::openRecordingValveForElement(IRecordable* src)
{
	if (!src) {
		LoggingController::warning("Cannot open recording valve for source: source is not recordable");
		return false;
	}
	
	return src->startRecording();
}

bool SessionPipeline::closeRecordingValveForElement(IRecordable* src)
{
	if (!src) {
		LoggingController::warning("Cannot close recording valve for source: source is not recordable");
		return false;
	}

	return src->stopRecording();
}

bool SessionPipeline::openProcessingValveForElement(Processor* proc)
{
	if (!proc) {
		LoggingController::warning("Cannot close valve for processor: processor is null");
		return false;
	}

	proc->startProcessing();
	return true;}

bool SessionPipeline::closeProcessingValveForElement(Processor* proc)
{
	if (!proc) {
		LoggingController::warning("Cannot close valve for processor: processor is null");
		return false;
	}

	proc->stopProcessing();
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
		LoggingController::warning("Invalid session state transition attempted.");
		return;
	}

	m_state = newState;
	emit stateChanged(newState);
}

void SessionPipeline::onPipelineError(const QString& errorMessage) {
	LoggingController::warning("Pipeline error occurred: " + errorMessage);
	setState(State::ERROR);
	emit errorOccurred(errorMessage);
}

void SessionPipeline::onPipelineEos() {
	LoggingController::debug("Pipeline EOS reached.");
	emit eosReached();
}