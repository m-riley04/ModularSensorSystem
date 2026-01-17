#include <pipeline/branches/intermediaries/previewcompositor.hpp>
#include <controllers/loggingcontroller.hpp>
#include <utils/debug.hpp>

PreviewCompositor::PreviewCompositor(GstPipeline* pipeline, GstElement* mainTee, PreviewBranch* previewBranch)
	: m_pipeline(pipeline)
	, m_mainTee(mainTee)
	, m_previewBranch(previewBranch)
	, m_compositor(gst_element_factory_make("input-selector", "preview_compositor"))
	, m_queuePreview(gst_element_factory_make("queue", "preview_queue"))
	, m_queueProcessor(nullptr)
	, m_processingBranch(nullptr)
{
	if (!m_pipeline || !m_mainTee || !m_previewBranch || !m_queuePreview || !m_compositor) {
		LoggingController::warning("Failed to create element(s) for PreviewCompositor.");
		return;
	}

	this->linkBaseElements();
}

PreviewCompositor::~PreviewCompositor()
{
	cleanup();
}

bool PreviewCompositor::linkBaseElements()
{
	if (!m_pipeline || !m_mainTee || !m_queuePreview || !m_compositor || !m_previewBranch) {
		LoggingController::warning("Cannot link preview branch in PreviewCompositor: one or more elements are null.");
		return false;
	}

	// TODO: check if some already added?
	gst_bin_add_many(GST_BIN(m_pipeline), m_queuePreview, m_compositor, nullptr);

	// Link main tee to compositor
	// TODO: check if already linked?
	if (!gst_element_link_many(m_mainTee, m_queuePreview, m_compositor, nullptr)) {
		LoggingController::warning("Failed to link PreviewBranch to PreviewCompositor.");
		return false;
	}

	// Link compositor to preview branch
	if (!gst_element_link(m_compositor, m_previewBranch->bin())) {
		LoggingController::warning("Failed to link PreviewCompositor to PreviewBranch.");
		return false;
	}

	return true;
}

bool PreviewCompositor::linkProcessingBranch(ProcessingBranch* processingBranch)
{
	// Check if processing queue exists
	if (!m_queueProcessor) m_queueProcessor = gst_element_factory_make("queue", "processing_queue");
	if (!gst_bin_add(GST_BIN(m_pipeline), m_queueProcessor)) {
		LoggingController::warning("Failed to add processing queue to pipeline in PreviewCompositor.");
		return false;
	}

	if (!m_pipeline || !m_queueProcessor || !m_compositor || !processingBranch) {
		LoggingController::warning("Cannot link preview branch in PreviewCompositor: one or more elements are null.");
		return false;
	}

	m_processingBranch = processingBranch;

	// Link processing branch to compositor
	if (!gst_element_link_many(m_processingBranch->bin(), m_queueProcessor, m_compositor, nullptr)) {
		LoggingController::warning("Failed to link ProcessingBranch to PreviewCompositor.");
		return false;
	}

	// Set the active pad to be the processing branch initially
	GstPad* activePad = gst_element_get_static_pad(m_compositor, "sink_1");
	g_object_set(m_compositor,
		"active-pad", activePad,
		nullptr);
	gst_object_unref(activePad);

	return true;
}

void PreviewCompositor::cleanup()
{
	if (m_queueProcessor) {
		gst_element_set_state(m_queueProcessor, GST_STATE_NULL);
	}
	if (m_queuePreview) {
		gst_element_set_state(m_queuePreview, GST_STATE_NULL);
	}
	if (m_compositor) {
		gst_element_set_state(m_compositor, GST_STATE_NULL);
	}

	if (m_pipeline) {
		if (m_queueProcessor && gst_object_has_as_parent(GST_OBJECT(m_queueProcessor), GST_OBJECT(m_pipeline))) {
			gst_bin_remove(GST_BIN(m_pipeline), m_queueProcessor);
		}
		if (m_queuePreview && gst_object_has_as_parent(GST_OBJECT(m_queuePreview), GST_OBJECT(m_pipeline))) {
			gst_bin_remove(GST_BIN(m_pipeline), m_queuePreview);
		}
		if (m_compositor && gst_object_has_as_parent(GST_OBJECT(m_compositor), GST_OBJECT(m_pipeline))) {
			gst_bin_remove(GST_BIN(m_pipeline), m_compositor);
		}
	}

	if (m_queueProcessor) {
		gst_object_unref(m_queueProcessor);
		m_queueProcessor = nullptr;
	}
	if (m_queuePreview) {
		gst_object_unref(m_queuePreview);
		m_queuePreview = nullptr;
	}
	if (m_compositor) {
		gst_object_unref(m_compositor);
		m_compositor = nullptr;
	}

	m_processingBranch = nullptr;
	m_previewBranch = nullptr;
	m_pipeline = nullptr;
	m_mainTee = nullptr;
}

