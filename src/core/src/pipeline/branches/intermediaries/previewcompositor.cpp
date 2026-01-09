#include <pipeline/branches/intermediaries/previewcompositor.hpp>
#include <controllers/loggingcontroller.hpp>
#include <utils/debug.hpp>

PreviewCompositor::PreviewCompositor(GstPipeline* pipeline, GstElement* mainTee)
	: m_pipeline(pipeline)
	, m_mainTee(mainTee)
	, m_compositor(nullptr)
	, m_queuePreview(nullptr)
	, m_queueProcessor(nullptr)
	, m_previewBranch(nullptr)
	, m_processingBranch(nullptr)
{
	m_queuePreview = gst_element_factory_make("queue", "preview_queue");
	m_queueProcessor = gst_element_factory_make("queue", "processor_queue");
	m_compositor = gst_element_factory_make("compositor", "preview_compositor");
	if (!m_queuePreview || !m_queueProcessor || !m_compositor) {
		LoggingController::warning("Failed to create element(s) for PreviewCompositor.");
	}
}

PreviewCompositor::~PreviewCompositor()
{

}

bool PreviewCompositor::linkBranches(PreviewBranch* previewBranch, ProcessingBranch* processingBranch)
{
	m_previewBranch = previewBranch;
	m_processingBranch = processingBranch;
	if (!m_queuePreview || !m_queueProcessor || !m_compositor || !m_previewBranch || !m_processingBranch) {
		LoggingController::warning("Cannot link branches in PreviewCompositor: one or more elements are null.");
		return false;
	}

	// Add elements to pipeline bin
	gst_bin_add_many(GST_BIN(m_pipeline), m_queuePreview, m_queueProcessor, m_compositor, nullptr);

	// Link main tee to compositor
	if (!gst_element_link_many(m_mainTee, m_queuePreview, m_compositor, nullptr)) {
		LoggingController::warning("Failed to link PreviewBranch to PreviewCompositor.");
		return false;
	}

	// Link processing branch to compositor
	if (!gst_element_link_many(m_processingBranch->bin(), m_queueProcessor, m_compositor, nullptr)) {
		LoggingController::warning("Failed to link ProcessingBranch to PreviewCompositor.");
		return false;
	}

	// Finally, link compositor to preview branch
	if (!gst_element_link(m_compositor, m_previewBranch->bin())) {
		LoggingController::warning("Failed to link PreviewCompositor to PreviewBranch.");
		return false;
	}

	// Configure pads
	// Set zorder for preview branch to be on top
	GstPad* previewPad = gst_element_get_static_pad(m_compositor, "sink_0");
	g_object_set(G_OBJECT(previewPad), "zorder", 99, nullptr);
	gst_object_unref(previewPad);

	// Set zorder for processing branch to be below preview
	GstPad* processingPad = gst_element_get_static_pad(m_compositor, "sink_1");
	g_object_set(G_OBJECT(processingPad), "zorder", 1, nullptr);
	gst_object_unref(processingPad);

	return true;
}
