#pragma once

#include <gst/gst.h>
#include <vector>
#include <pipeline/branches/teebranch.hpp>
#include <pipeline/branches/previewbranch.hpp>

/**
 * @brief An intermediary that composites the preview and processing branches together, along with the base stream.
 * NOT contained within a bin (for now).
 */
class PreviewCompositor {
private:
	GstElement* m_compositor;
	GstElement* m_queuePreview;
	GstElement* m_queueProcessor;
	PreviewBranch* m_previewBranch;
	ProcessingBranch* m_processingBranch;
	GstPipeline* m_pipeline;
	GstElement* m_mainTee;

	bool linkBaseElements();
public:
	PreviewCompositor(GstPipeline* pipeline, GstElement* mainTee, PreviewBranch* previewBranch);
	~PreviewCompositor();

	bool setProcessingEnabled(bool enabled);

	bool linkProcessingBranch(ProcessingBranch* processingBranch);
	void cleanup();
};