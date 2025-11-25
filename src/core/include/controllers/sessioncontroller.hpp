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
#include "pipeline/sinks/preview_defaults.hpp"
#include <interfaces/capability/ipreviewablesource.hpp>
#include "models/session_properties.hpp"
#include <utils/debug.hpp>
#include <utils/utils.hpp>
#include <utils/session_utils.hpp>
#include <QRegularExpression>

#define DEFAULT_SESSION_PREFIX "session_"
#define DEFAULT_SESSIONS_DIRECTORY "/sessions"

using OneToManyIdMap = QHash<QUuid, std::vector<QUuid>>;

constexpr const char* MAIN_PIPELINE_NAME = "main_pipeline";

class SessionController : public BackendControllerBase
{
	Q_OBJECT

public:
	enum class State {
		STARTING,
		STARTED,
		STOPPING,
		STOPPED,
		RECORDING,
		STOPPING_RECORDING,
		ERROR
	};

public:
	SessionController(SourceController* sourceController, ProcessingController* processingController, 
		MountController* mountController, QObject* parent);
	~SessionController();

	GstPipeline* pipeline() const { return m_pipeline.get(); }
	bool isPipelineBuilt() const { return m_pipeline != nullptr; }
	SessionProperties& sessionProperties() const { return *m_sessionProperties; }
	
	QList<const Source*> getSourcesByMount(QUuid mountId) const;
	QList<const Processor*> getProcessorsBySource(QUuid sourceId) const;

	// States
	State state() const { return m_state; }
	bool isStarting() const { return m_state == State::STARTING; }
	bool isStarted() const { return m_state == State::STARTED; }
	bool isStoppingSession() const { return m_state == State::STOPPING; }
	bool isStopped() const { return m_state == State::STOPPED; }
	bool isRecording() const { return m_state == State::RECORDING; }
	bool isStoppingRecording() const { return m_state == State::STOPPING_RECORDING; }

public slots:
	void startSession();
	void requestStopSession();
	void stopSession();
	void restartSession();

	void startRecording();
	void requestStopRecording();
	void stopRecording();
	
	void setState(State newState);
	void setPipelineError(const QString& errorMessage);
	void setSessionProperties(SessionProperties properties);

private:
	std::unique_ptr<GstPipeline, decltype(&gst_object_unref)> m_pipeline;
	guint m_pipelineBusWatchId = 0;
	ns m_lastSessionTimestamp = 0;
	ns m_lastRecordingTimestamp = 0;
	State m_state = State::STOPPED;

	QList<GstElement*> m_sourceBins;
	QList<GstElement*> m_previewBins;
	QList<GstElement*> m_recordBins;
	QList<GstElement*> m_recordableSources;

	SessionProperties* m_sessionProperties = nullptr;

	SourceController* m_sourceController = nullptr;
	ProcessingController* m_processingController = nullptr;
	MountController* m_mountController = nullptr;

	OneToManyIdMap m_mountToSources;
	OneToManyIdMap m_sourceToProcessors;

	void buildPipeline();
	void closePipeline();

	gboolean createSourceElements(Source* source);
	gboolean createAndLinkPreviewBin(Source* src, GstElement* srcBin);
	gboolean createAndLinkRecordBin(Source* src, GstElement* srcBin);

	gboolean openRecordingValves();
	gboolean closeRecordingValves();
	gboolean openRecordingValveForSource(Source* source);
	gboolean closeRecordingValveForSource(Source* source);

signals:
	void sessionStarting();
	void sessionStarted();
	void sessionStopping();
	void sessionStopped();
	void sessionRestarted();
	void stateChanged(SessionController::State newState);

	void pipelineEosReached();

	void sessionPropertiesChanged(SessionProperties properties);
	void errorOccurred(QString errorMessage);

	void recordingStarted();
	void recordingStopping();
	void recordingStopped();

};