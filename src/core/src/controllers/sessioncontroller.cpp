#include "controllers/sessioncontroller.h"

SessionController::SessionController(QObject* parent)
	: BackendControllerBase("SessionController", parent)
{
	if (!gst_is_initialized()) {
		gst_init(nullptr, nullptr);
	}

	
}

SessionController::~SessionController()
{
	if (m_pipeline != nullptr) gst_object_unref(m_pipeline);
}

void SessionController::buildPipeline()
{
	// Check if the pipeline is already built
	if (m_pipeline != nullptr) {
		gst_object_unref(m_pipeline);
	}

	m_pipeline = GST_PIPELINE(gst_pipeline_new("main_pipeline"));
}
