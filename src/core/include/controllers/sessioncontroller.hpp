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

using OneToManyIdMap = QHash<QUuid, std::vector<QUuid>>;

constexpr const char* MAIN_PIPELINE_NAME = "main_pipeline";

class SessionController : public BackendControllerBase
{
	Q_OBJECT

public:
	SessionController(SourceController* sourceController, ProcessingController* processingController, 
		MountController* mountController, QObject* parent);
	~SessionController();

	/**
	 * Builds the main data pipeline through GStreamer.
	 */
	void buildPipeline();

	/**
	 * Closes the main data pipeline and releases resources.
	 */
	void closePipeline();

	GstPipeline* pipeline() const { return m_pipeline.get(); }

	bool isPipelineBuilt() const { return m_pipeline != nullptr; }

	QList<const Source*> getSourcesByMount(QUuid mountId) const;
	QList<const Processor*> getProcessorsBySource(QUuid sourceId) const;

private:
	std::unique_ptr<GstPipeline, decltype(&gst_object_unref)> m_pipeline;

	SourceController* m_sourceController = nullptr;
	ProcessingController* m_processingController = nullptr;
	MountController* m_mountController = nullptr;

	OneToManyIdMap m_mountToSources;
	OneToManyIdMap m_sourceToProcessors;

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

	void dataSampleReceived(AnalogDataSample sample);
};