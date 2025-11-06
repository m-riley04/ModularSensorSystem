#pragma once

#include "controllers/backendcontrollerbase.h"
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <QObject>
#include <QDebug>
#include "controllers/sourcecontroller.h"
#include "controllers/processingcontroller.h"
#include "controllers/mountcontroller.h"
#include "features/sources/source.h"

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

	void createSourceElements(Source* source);
	void createVideoSourceElements(Source* source);
	void createAudioSourceElements(Source* source);
	void createDataSourceElements(Source* source);

signals:
	void sessionStarted();
	void sessionStopped();
};