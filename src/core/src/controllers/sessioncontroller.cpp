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

void SessionController::createSourceElements(Source* source)
{
	// Check source
	if (!source) {
		qWarning() << "Cannot create source elements: source is null";
		return;
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

void SessionController::createVideoSourceElements(Source* source)
{
	const char* sinkName = getVideoSinkFactoryName();

	// Initialize elements
	GstElement* gstBin = source->gstBin();
	GstElement* sink = gst_element_factory_make(sinkName, NULL); // TODO: make this dynamic
	guintptr windowId = static_cast<guintptr>(source->windowId());

	// Check validity of each
	if (!gstBin || !sink) {
		qWarning() << "Failed to create one or more elements";
		if (sink) gst_object_unref(sink);
		return;
	}

	// Add elements to pipeline
	gst_bin_add_many(GST_BIN(m_pipeline.get()), gstBin, sink, NULL);

	// Link source bin to sink
	if (!gst_element_link(gstBin, sink)) {
		qWarning() << "Failed to link source bin to sink.";
		gst_bin_remove_many(GST_BIN(m_pipeline.get()), gstBin, sink, NULL);
		return;
	}

	// Set the video sink for overlay
	if (windowId != 0 && GST_IS_VIDEO_OVERLAY(sink)) {
		gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(sink), windowId);
	}
}

void SessionController::createAudioSourceElements(Source* source)
{
	/** Multiple differet visualizers could be used here.
	 * - monoscope (best one by far)
	 * - goom (cool visuals, but not really good for real visualization)
	 * - audiovisualizations (these are a set of visualizers, but are not very good and pretty buggy on some systems)
	 *     - spacescope
	 *     - specrtascope
	 *     - synaescope
	 *     - wavescope
	 * - libvisual(?)
	 * 
	 * TODO: implement a way to choose between them. */
	const char* visualizerFactoryName = "monoscope";
	const char* sinkName = getVideoSinkFactoryName();

	// Initialize elements
	GstElement* gstBin = source->gstBin();
	GstElement* wavescope = gst_element_factory_make(visualizerFactoryName, NULL);
	GstElement* conv = gst_element_factory_make("videoconvert", NULL);
	GstElement* queue = gst_element_factory_make("queue", NULL);
	GstElement* sink = gst_element_factory_make(sinkName, NULL);
	guintptr windowId = static_cast<guintptr>(source->windowId());

	// Check validity of each
	if (!gstBin || !sink || !wavescope || !conv || !queue) {
		qWarning() << "Failed to create one or more elements";
		if (sink) gst_object_unref(sink);
		if (wavescope) gst_object_unref(wavescope);
		if (conv) gst_object_unref(conv);
		if (queue) gst_object_unref(queue);
		return;
	}

	// Add elements to pipeline
	gst_bin_add_many(GST_BIN(m_pipeline.get()), gstBin, wavescope, conv, queue, sink, NULL);

	// Link source bin to elements
	if (!gst_element_link_many(gstBin, wavescope, conv, queue, sink, NULL)) {
		qWarning() << "Failed to link source bin to elements.";
		gst_bin_remove_many(GST_BIN(m_pipeline.get()), gstBin, wavescope, conv, queue, sink, NULL);
		return;
	}

	// Set the video sink for overlay
	if (windowId != 0 && GST_IS_VIDEO_OVERLAY(sink)) {
		gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(sink), windowId);
	}
}

void SessionController::createDataSourceElements(Source* source)
{
	// Check source
	if (!source) {
		qWarning() << "Cannot create data source elements: source is null";
		return;
	}

	// Create appsink for data
	GstElement* sink = gst_element_factory_make("appsink", nullptr);
	if (!sink) {
		qWarning() << "Failed to create appsink for data source";
		return;
	}

	// Configure appsink: emit signals, don't sync to clock, bounded queue
	g_object_set(G_OBJECT(sink),
		"emit-signals", TRUE,
		"sync", TRUE,
		"max-buffers", 100u,
		"drop", TRUE,
		nullptr);

	// Optionally restrict caps to your NDJSON if you like:
	// GstCaps* caps = gst_caps_from_string("application/x-mss-random-ndjson");
	// g_object_set(G_OBJECT(sink), "caps", caps, nullptr);
	// gst_caps_unref(caps);

	GstElement* gstBin = source->gstBin();
	if (!gstBin) {
		qWarning() << "Data source has no Gst bin";
		gst_object_unref(sink);
		return;
	}

	gst_bin_add_many(GST_BIN(m_pipeline.get()), gstBin, sink, nullptr);

	// Link source bin to appsink's sink pad
	if (!gst_element_link(gstBin, sink)) {
		qWarning() << "Failed to link data source bin to appsink";
		gst_bin_remove_many(GST_BIN(m_pipeline.get()), gstBin, sink, nullptr);
		return;
	}

	// Connect "new-sample" to our static callback
	// userData = this, we will parse sensor_id out of JSON
	g_signal_connect(sink, "new-sample", G_CALLBACK(&SessionController::onDataNewSampleStatic), this);
}

void SessionController::closePipeline()
{
	if (!m_pipeline) return;
	gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_NULL);

	emit sessionStopped();

	// Should go after emit to allow sources to clean up
	m_pipeline.reset(nullptr);
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

GstFlowReturn SessionController::onDataNewSampleStatic(GstAppSink* sink, gpointer userData)
{
	auto* self = static_cast<SessionController*>(userData);
	return self ? self->onDataNewSample(sink) : GST_FLOW_ERROR;
}

GstFlowReturn SessionController::onDataNewSample(GstAppSink* sink)
{
	GstSample* sample = gst_app_sink_pull_sample(sink);
	if (!sample)
		return GST_FLOW_ERROR;

	GstBuffer* buffer = gst_sample_get_buffer(sample);
	if (!buffer) {
		gst_sample_unref(sample);
		return GST_FLOW_ERROR;
	}

	GstMapInfo map;
	if (!gst_buffer_map(buffer, &map, GST_MAP_READ)) {
		gst_sample_unref(sample);
		return GST_FLOW_ERROR;
	}

	QByteArray payload(reinterpret_cast<const char*>(map.data),
		static_cast<int>(map.size));

	gst_buffer_unmap(buffer, &map);

	// Optional: running-time PTS in ns (if appsink caps/format support it)
	GstClockTime pts = GST_BUFFER_PTS(buffer);
	quint64 tNs = (pts == GST_CLOCK_TIME_NONE) ? 0 : static_cast<quint64>(pts);

	gst_sample_unref(sample);

	// Our TestDataSourceBin emits NDJSON, possibly with multiple lines
	const auto lines = payload.split('\n');
	for (const QByteArray& line : lines) {
		const QByteArray trimmed = line.trimmed();
		if (trimmed.isEmpty())
			continue;

		QJsonParseError err{};
		QJsonDocument doc = QJsonDocument::fromJson(trimmed, &err);
		if (err.error != QJsonParseError::NoError || !doc.isObject())
			continue;

		QJsonObject obj = doc.object();
		const QString uuidStr = obj.value(QStringLiteral("uuid")).toString();
		QUuid uuid(uuidStr);
		const double value = obj.value(QStringLiteral("value")).toDouble(
			std::numeric_limits<double>::quiet_NaN());

		// Forward into Qt world
		QMetaObject::invokeMethod(
			this,
			[this, uuid, value, tNs]() {
				AnalogDataSample sample = {
					uuid,
					value,
					tNs
				};
				emit dataSampleReceived(sample);
			},
			Qt::QueuedConnection);
	}

	return GST_FLOW_OK;
}

const char* getVideoSinkFactoryName()
{
	// TODO: Make this better and more flexible?
	const char* sinkName;
#ifdef Q_OS_WINDOWS
	sinkName = "d3dvideosink";
#elif Q_OS_LINUX
	sinkName = "v4l2sink";
#else
	sinkName = "autovideosink"; // need to change this probs
#endif
	return sinkName;
}