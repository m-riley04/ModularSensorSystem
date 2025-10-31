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

	// TODO/DEBUG: this is just a test pipeline to get video output working
	GstElement* source = gst_element_factory_make("videotestsrc", NULL);
	GstElement* sink = gst_element_factory_make("autovideosink", NULL); // Or xvimagesink, ximagesink, etc.

	// Add elements to pipeline
	gst_bin_add_many(GST_BIN(m_pipeline.get()), source, sink, NULL);

	// Link elements
	gst_element_link(source, sink);

	// TODO/DEBUG: set the video sink for overlay
	gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(m_videoSink), static_cast<guintptr>(m_sinkWindowId));

	// Set state to playing
	gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_READY);
	gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_PLAYING);
}

void SessionController::closePipeline()
{
	// Reset state and release resources
	gst_element_set_state(GST_ELEMENT(m_pipeline.get()), GST_STATE_NULL);
	m_pipeline.reset();
}

