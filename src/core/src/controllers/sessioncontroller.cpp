#include "controllers/sessioncontroller.hpp"
#include <utils/debug.hpp>

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

	GstState current, pending;
	GstStateChangeReturn sret = gst_element_get_state(
		GST_ELEMENT(m_pipeline.get()), &current, &pending, 5 * GST_SECOND);

	qDebug() << "Pipeline state change result:"
		<< sret
		<< "current:"
		<< gst_element_state_get_name(current)
		<< "pending:"
		<< gst_element_state_get_name(pending);
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
	guintptr windowId = static_cast<guintptr>(source->windowId());

	// Initialize elements
	GstElement* gstBin = source->gstBin();
	GstElement* sink = createDefaultVideoSink(windowId);

	// Check validity of each
	if (!gstBin || !sink) {
		qWarning() << "Failed to create one or more elements";
		if (sink) gst_object_unref(sink);
		return;
	}

	// Add elements to pipeline
	gst_bin_add_many(GST_BIN(m_pipeline.get()), gstBin, sink, nullptr);

	// Link source bin to sink
	if (!gst_element_link(gstBin, sink)) {
		qWarning() << "Failed to link source bin to sink.";
		gst_bin_remove_many(GST_BIN(m_pipeline.get()), gstBin, sink, nullptr);
		return;
	}
}

void SessionController::createAudioSourceElements(Source* source)
{
	guintptr windowId = static_cast<guintptr>(source->windowId());

	// Init elements
	GstElement* gstBin = source->gstBin();
	GstElement* sinkBin = createDefaultAudioVisualizerSink(windowId);

	// Check validity of each
	if (!gstBin || !sinkBin) {
		qWarning() << "Failed to create one or more elements";
		if (gstBin) gst_object_unref(gstBin);
		if (sinkBin) gst_object_unref(sinkBin);
		return;
	}

	// Add elements to pipeline
	gst_bin_add_many(GST_BIN(m_pipeline.get()), gstBin, sinkBin, nullptr);

	// Link source bin to elements
	if (!gst_element_link(gstBin, sinkBin)) {
		qWarning() << "Failed to link source bin to elements.";
		gst_bin_remove_many(GST_BIN(m_pipeline.get()), gstBin, sinkBin, nullptr);
		return;
	}
}

void SessionController::createDataSourceElements(Source* source)
{
	// Check source
	if (!source) {
		qWarning() << "Cannot create data source elements: source is null";
		return;
	}

	GstElement* gstBin = source->gstBin();
	if (!gstBin) {
		qWarning() << "Data source has no Gst bin";
		return;
	}

	// Visualize doubles using our custom element and a videosink
	GstElement* visualizerBin = createDefaultDataVisualizerSink(static_cast<guintptr>(source->windowId()));
	if (!visualizerBin) {
		qWarning() << "Failed to create data visualizer bin (is the custom plugin registered and a video sink available?)";
		return;
	}

	// Optionally restrict caps to your NDJSON if you like:
	// GstCaps* caps = gst_caps_from_string("application/x-double");
	// g_object_set(G_OBJECT(sink), "caps", caps, nullptr);
	// gst_caps_unref(caps);

	// Add both to pipeline before linking
	gst_bin_add_many(GST_BIN(m_pipeline.get()), gstBin, visualizerBin, nullptr);

	// Link source bin to appsink's sink pad
	if (!gst_element_link(gstBin, visualizerBin)) {
		qWarning() << "Failed to link data source bin to appsink";
		gst_bin_remove_many(GST_BIN(m_pipeline.get()), gstBin, visualizerBin, nullptr);
		return;
	}
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
