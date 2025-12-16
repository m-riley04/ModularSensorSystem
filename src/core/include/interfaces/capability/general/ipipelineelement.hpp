#pragma once

#include <gst/gst.h>

class IPipelineElement {

public:
	virtual ~IPipelineElement() = default;

	/**
	 * @brief Returns the GStreamer bin element that provides data to the pipeline.
	 * @return Pointer to the GstElement representing the source bin. Nullptr if not applicable.
	 */
	virtual GstElement* gstSrcBin() = 0;

	/**
	 * @brief Returns the GStreamer bin element used for processing data in the pipeline.
	 * @return Pointer to the GstElement representing the filter bin. Nullptr if not applicable.
	 */
	virtual GstElement* gstFilterBin() = 0;

	/**
	 * @brief Returns the gstreamer bin element that receives data from the pipeline.
	 * @return Pointer to the GstElement representing the sink bin. Nullptr if not applicable.
	 */
	virtual GstElement* gstSinkBin() = 0;
};