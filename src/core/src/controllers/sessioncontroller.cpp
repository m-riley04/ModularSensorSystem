#include "controllers/sessioncontroller.hpp"
#include <utils/debug.hpp>
#include <QRegularExpression>

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

long long SessionController::generateSessionTimestamp()
{
	// Get the current time point from the high-resolution clock
	std::chrono::time_point<std::chrono::high_resolution_clock> now =
		std::chrono::high_resolution_clock::now();

	// Get the duration since the clock's epoch
	auto duration_since_epoch = now.time_since_epoch();

	// Cast the duration to nanoseconds
	std::chrono::nanoseconds nanoseconds_duration =
		std::chrono::duration_cast<std::chrono::nanoseconds>(duration_since_epoch);

	// Get the count of nanoseconds
	long long nanoseconds_count = nanoseconds_duration.count();

	return nanoseconds_count;
}

static QString sanitizeFileName(const QString& name)
{
	// Replace characters that are invalid on Windows file systems
	// Invalid: \ / : * ? " < > |
	QString cleaned;
	cleaned.reserve(name.size());
	for (QChar ch : name) {
		if (ch == '\\' || ch == '/' || ch == ':' || ch == '*' || ch == '?' || ch == '"' || ch == '<' || ch == '>' || ch == '|') {
			cleaned.append('_');
		} else {
			cleaned.append(ch);
		}
	}
	// Remove trailing dots and spaces which are not allowed for filenames on Windows
	while (!cleaned.isEmpty() && (cleaned.endsWith('.') || cleaned.endsWith(' '))) {
		cleaned.chop(1);
	}
	if (cleaned.isEmpty()) {
		cleaned = QStringLiteral("source");
	}
	return cleaned;
}

QString SessionController::generateSessionDirectoryPath()
{
	const QString outputDir = m_sessionProperties->recordingProperties.outputDirectory.absolutePath();

	// Check output directory
	if (!QDir(outputDir).exists()) {
		if (!QDir().mkpath(outputDir)) {
			qWarning() << "Failed to create base output directory:" << outputDir;
			return QString();
		}
	}

	const QString outputFolderPrefix = m_sessionProperties->recordingProperties.outputPrefix + QString::fromStdString(std::to_string(m_lastSessionTimestamp));
	const QString outputFolderPath = outputDir + "/" + outputFolderPrefix;

	// Check output directory
	if (!QDir(outputFolderPath).exists()) {
		if (!QDir().mkpath(outputFolderPath)) {
			qWarning() << "Failed to create session output directory:" << outputFolderPath;
			return QString();
		}
	}

	return outputFolderPath;
}

QString SessionController::generateSessionSourcePath(Source* src)
{
	auto recordableSrc = src->asRecordable();
	if (!src->asRecordable()) {
		qWarning() << "Cannot generate session source path: source is not recordable:" << QString::fromStdString(src->name());
		return QString();
	}

	const QString outputFolderPath = QDir::cleanPath(generateSessionDirectoryPath());

	// Check output folder path
	if (outputFolderPath.isEmpty()) {
		qWarning() << "Cannot generate session source path: output folder path is empty for source:" << QString::fromStdString(src->name());
		return QString();
	}

	// Sanitize the file name derived from the source display/name to avoid invalid characters
	QString baseName = sanitizeFileName(QString::fromStdString(src->name())).replace(" ", "_");

	// Avoid reserved DOS device names
	static const QStringList reserved = {
		QStringLiteral("CON"), QStringLiteral("PRN"), QStringLiteral("AUX"), QStringLiteral("NUL"),
		QStringLiteral("COM1"), QStringLiteral("COM2"), QStringLiteral("COM3"), QStringLiteral("COM4"),
		QStringLiteral("COM5"), QStringLiteral("COM6"), QStringLiteral("COM7"), QStringLiteral("COM8"), QStringLiteral("COM9"),
		QStringLiteral("LPT1"), QStringLiteral("LPT2"), QStringLiteral("LPT3"), QStringLiteral("LPT4"),
		QStringLiteral("LPT5"), QStringLiteral("LPT6"), QStringLiteral("LPT7"), QStringLiteral("LPT8"), QStringLiteral("LPT9")
	};
	if (reserved.contains(baseName.toUpper())) {
		baseName.prepend('_');
	}

	QString extension = QString::fromStdString(recordableSrc->recorderFileExtension());
	if (extension.startsWith('.')) {
		extension.remove(0, 1);
	}

	// Conservative MAX_PATH handling: keep full path well below 260
	const int maxFull = 250; // leave headroom
	QString sep = QStringLiteral("/");
	QString suffix = extension.isEmpty() ? QString() : QStringLiteral(".") + extension;
	QString candidate = outputFolderPath + sep + baseName + suffix;
	if (candidate.size() > maxFull) {
		int budget = maxFull - (outputFolderPath.size() + 1 + suffix.size());
		if (budget < 8) budget = 8; // minimum reasonable
		baseName = baseName.left(budget);
		candidate = outputFolderPath + sep + baseName + suffix;
	}

	const QString cleanedOutputFilePath = candidate.trimmed();

	if (cleanedOutputFilePath.isEmpty()) {
		qWarning() << "Cannot generate session source path: output file path is empty for source:" << QString::fromStdString(src->name());
		return QString();
	}

	return cleanedOutputFilePath;
}

static gboolean
bus_call(GstBus* bus,
	GstMessage* msg,
	gpointer    data)
{
	SessionController* sessionController = (SessionController*)data;

	switch (GST_MESSAGE_TYPE(msg)) {

	case GST_MESSAGE_EOS:
		g_print("End of stream\n");
		sessionController->notifyPipelineEOS();
		sessionController->stopSession();
		break;

	case GST_MESSAGE_WARNING: {
		gchar* debug;
		GError* error;
		gst_message_parse_warning(msg, &error, &debug);
		g_free(debug);
		QMessageLogger().warning("GStreamer Warning: %s", error->message);
		g_error_free(error);
		break;
	}
	case GST_MESSAGE_UNKNOWN:
		g_print("Received unknown message.\n");
		break;
	case GST_MESSAGE_ERROR: {
		gchar* debug;
		GError* error;

		gst_message_parse_error(msg, &error, &debug);
		g_free(debug);

		// Notify session controller of the error
		sessionController->notifyPipelineError(QString::fromUtf8(error->message));
		g_error_free(error);

		break;
	}
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
	m_lastSessionTimestamp = generateSessionTimestamp();

	// Add bus watch
	GstBus* bus;
	bus = gst_pipeline_get_bus(m_pipeline.get());
	m_pipelineBusWatchId = gst_bus_add_watch(bus, bus_call, this);
	gst_object_unref(bus);

	// Iterate over all sources and add them
	for (auto& src : m_sourceController->sources()) {
		createSourceElements(src);

		// Link start and stop hooks
		connect(this, &SessionController::sessionStarted, src, &Source::onSessionStart);
		connect(this, &SessionController::sessionStopped, src, &Source::onSessionStop);
	}

	// DEBUG: export pipeline graph
	//debugDisplayGstBin(GST_ELEMENT(m_pipeline.get()), true);
	
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

void SessionController::closePipeline()
{
	if (!m_pipeline) return;
	gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_NULL);
	g_source_remove(m_pipelineBusWatchId);
	m_pipelineBusWatchId = 0;

	emit sessionStopped();

	// Should go after emit to allow sources to clean up
	m_pipeline.reset(nullptr);
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
	const QString outputFilePath = generateSessionSourcePath(src);
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
		recSrc->openRecordingValve();
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
		recSrc->closeRecordingValve();
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

void SessionController::startSession()
{
	buildPipeline();
	emit sessionStarted();
}

void SessionController::stopSession()
{
	closePipeline();
	emit sessionStopped();
}

void SessionController::startRecording()
{
	if (!openRecordingValves()) {
		return;
	}

	emit recordingStarted();
}

void SessionController::stopRecording()
{
	if (!closeRecordingValves()) {
		return;
	}

	emit recordingStopped();
}

void SessionController::notifyPipelineEOS()
{
	emit pipelineEOSReached();
}

void SessionController::notifyPipelineError(const QString& errorMessage)
{
	emit pipelineErrorOccurred(errorMessage);
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
