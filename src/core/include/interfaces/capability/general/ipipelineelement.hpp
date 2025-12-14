#pragma once

#include <gst/gst.h>

class IPipelineElement {

public:
	virtual ~IPipelineElement() = default;

	/**
	 * @brief Returns the GStreamer source bin element for this pipeline element.
	 * @return Pointer to the GstElement representing the source bin.
	 */
	virtual GstElement* gstSrcBin() = 0;
};