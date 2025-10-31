#pragma once

#include "controllers/backendcontrollerbase.h"
#include <gst/gst.h>
#include <QObject>

constexpr const char* MAIN_PIPELINE_NAME = "main_pipeline";

class SessionController : public BackendControllerBase
{
	Q_OBJECT

public:
	SessionController(QObject* parent);
	~SessionController();

	/**
	 * Builds the main data pipeline through GStreamer.
	 */
	void buildPipeline();

	/**
	 * Closes the main data pipeline and releases resources.
	 */
	void closePipeline();

	bool isPipelineBuilt() const { return m_pipeline != nullptr; }

private:
	std::unique_ptr<GstPipeline, decltype(&gst_object_unref)> m_pipeline;
};