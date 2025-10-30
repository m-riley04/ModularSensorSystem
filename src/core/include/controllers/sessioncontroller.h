#pragma once

#include "controllers/backendcontrollerbase.h"
#include <gst/gst.h>
#include <QObject>

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

private:
	GstPipeline* m_pipeline;
};