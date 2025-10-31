#include "controllers/sessioncontroller.h"

SessionController::SessionController(QObject* parent)
	: BackendControllerBase("SessionController", parent), m_pipeline(nullptr, &gst_object_unref)
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
	// If a pipeline already exists, close it first and make a new one
	m_pipeline.reset(GST_PIPELINE(gst_pipeline_new(MAIN_PIPELINE_NAME)));
}

void SessionController::closePipeline()
{
	m_pipeline.reset();
}
