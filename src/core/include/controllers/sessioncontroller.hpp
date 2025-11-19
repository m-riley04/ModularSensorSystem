#pragma once

#include <QObject>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <gst/app/gstappsink.h>
#include "controllers/backendcontrollerbase.hpp"
#include "controllers/sourcecontroller.hpp"
#include "controllers/processingcontroller.hpp"
#include "controllers/mountcontroller.hpp"
#include "features/sources/source.hpp"
#include "interfaces/capability/ianalogsource.hpp"
#include "pipeline/sinks/defaults.hpp"
#include <interfaces/capability/ipreviewablesource.hpp>
#include "models/session_properties.hpp"

#define DEFAULT_SESSION_PREFIX "session_"
#define DEFAULT_SESSIONS_DIRECTORY "/sessions"

using OneToManyIdMap = QHash<QUuid, std::vector<QUuid>>;

constexpr const char* MAIN_PIPELINE_NAME = "main_pipeline";

class SessionController : public BackendControllerBase
{
	Q_OBJECT

public:
	SessionController(SourceController* sourceController, ProcessingController* processingController, 
		MountController* mountController, QObject* parent);
	~SessionController();

	GstPipeline* pipeline() const { return m_pipeline.get(); }
	bool isPipelineBuilt() const { return m_pipeline != nullptr; }
	SessionProperties& sessionProperties() const { return *m_sessionProperties; }

	QList<const Source*> getSourcesByMount(QUuid mountId) const;
	QList<const Processor*> getProcessorsBySource(QUuid sourceId) const;

public slots:
	void startSession();
	void stopSession();
	void restartSession();

	void setSessionProperties(SessionProperties properties);

	void startRecording();
	void stopRecording();

	void notifyPipelineEOS();
	void notifyPipelineError(const QString& errorMessage);

private:
	std::unique_ptr<GstPipeline, decltype(&gst_object_unref)> m_pipeline;
	guint m_pipelineBusWatchId = 0;

	SessionProperties* m_sessionProperties = nullptr;

	SourceController* m_sourceController = nullptr;
	ProcessingController* m_processingController = nullptr;
	MountController* m_mountController = nullptr;

	OneToManyIdMap m_mountToSources;
	OneToManyIdMap m_sourceToProcessors;

	long long m_lastSessionTimestamp = 0;
	static long long generateSessionTimestamp();

	QString generateSessionDirectoryPath();
	QString generateSessionSourcePath(Source* src);

	/**
	 * Builds the main data pipeline through GStreamer.
	 */
	void buildPipeline();

	/**
	 * Closes the main data pipeline and releases resources.
	 */
	void closePipeline();

	gboolean createSourceElements(Source* source);
	gboolean createAndLinkPreviewBin(Source* src, GstElement* srcBin);
	gboolean createAndLinkRecordBin(Source* src, GstElement* srcBin);

	gboolean openRecordingValves();
	gboolean closeRecordingValves();
	gboolean openRecordingValveForSource(Source* source);
	gboolean closeRecordingValveForSource(Source* source);

	static GstFlowReturn onDataNewSampleStatic(GstAppSink* sink, gpointer userData);
	GstFlowReturn onDataNewSample(GstAppSink* sink);

signals:
	void sessionStarted();
	void sessionStopped();
	void sessionRestarted();

	void sessionPropertiesChanged(SessionProperties properties);

	void recordingStarted();
	void recordingStopped();

	void pipelineEOSReached();
	void pipelineErrorOccurred(QString errorMessage);

	void dataSampleReceived(AnalogDataSample sample);
};