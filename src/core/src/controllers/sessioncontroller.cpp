#include "controllers/sessioncontroller.h"

SessionController::SessionController(SourceController* sourceController, ProcessingController* processingController, 
	MountController* mountController, QObject* parent)
	: BackendControllerBase("SessionController", parent), m_pipeline(nullptr, &gst_object_unref), 
	m_sourceController(sourceController), m_processingController(processingController), 
	m_mountController(mountController)
{
	if (!gst_is_initialized()) {
		gst_init(nullptr, nullptr);
	}
}

SessionController::~SessionController()
{
	closePipeline();
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

	// Iterate over all sources and add them
	for (auto& src : m_sourceController->sources()) {
		SourceGstElements elements = createSourceElements(src);

		if (!isValidElements(elements)) {
			qWarning() << "Invalid source elements created";
			closePipeline();
			return;
		}
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
}
	
bool SessionController::isValidElements(SourceGstElements elements) const
{
	return elements.srcElement != nullptr && elements.sinkElement != nullptr && elements.sinkWindowId != 0;
}

SourceGstElements SessionController::createSourceElements(Source* source)
{
	// Check source
	if (!source) {
		qWarning() << "Cannot create source elements: source is null";
		return SourceGstElements();
	}

	switch (source->type()) {
	case Source::Type::AUDIO:
		return createAudioSourceElements(source);
	case Source::Type::VIDEO:
		return createVideoSourceElements(source);
	case Source::Type::DATA:
	default:
		return createDataSourceElements(source);
	}
}

SourceGstElements SessionController::createVideoSourceElements(Source* source)
{
	SourceGstElements elements;

	// Initialize source
	GstElement* sourceGst = gst_element_factory_make("mfvideosrc", NULL); // TODO: make this dynamic and cross-platform
	g_object_set(sourceGst, "device-path", source->id().c_str(), NULL);

	// Initialize queue and converter
	GstElement* queue = gst_element_factory_make("queue", NULL);
	GstElement* conv = gst_element_factory_make("videoconvert", NULL);

	// Initialize sink
	GstElement* sink = gst_element_factory_make("d3dvideosink", NULL); // TODO: make this dynamic
	guintptr windowId = static_cast<guintptr>(source->windowId());

	if (!sourceGst || !queue || !conv || !sink) {
		qWarning() << "Failed to create one or more elements";
		if (sourceGst)  gst_object_unref(sourceGst);
		if (queue) gst_object_unref(queue);
		if (conv) gst_object_unref(conv);
		if (sink) gst_object_unref(sink);
		return {};
	}

	// Add elements to pipeline
	gst_bin_add_many(GST_BIN(m_pipeline.get()), sourceGst, queue, conv, sink, NULL);

	// Link elements
	if (!gst_element_link_many(sourceGst, queue, conv, sink, nullptr)) {
		qWarning() << "Failed to link mfvideosrc -> queue -> videoconvert -> d3dvideosink";
		// Remove and unref on failure
		gst_bin_remove_many(GST_BIN(m_pipeline.get()), sourceGst, queue, conv, sink, nullptr);
		gst_object_unref(sourceGst); gst_object_unref(queue); gst_object_unref(conv); gst_object_unref(sink);
		return {};
	}

	// Set the video sink for overlay
	if (windowId != 0 && GST_IS_VIDEO_OVERLAY(sink)) {
        gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(sink), windowId);
    }

	// Assign to output
	elements.src = source;
	elements.srcElement = sourceGst;
	elements.queueElement = queue;
	elements.convElement = conv;
	elements.sinkElement = sink;
	elements.sinkWindowId = windowId;

	return elements;
}

SourceGstElements SessionController::createAudioSourceElements(Source* source)
{
	SourceGstElements elements;

	return elements;
}

SourceGstElements SessionController::createDataSourceElements(Source* source)
{
	SourceGstElements elements;

	return elements;
}

void SessionController::closePipeline()
{
	if (!m_pipeline) return;
	gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_NULL);
	m_pipeline.reset();
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