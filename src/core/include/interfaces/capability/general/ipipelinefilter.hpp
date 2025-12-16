#pragma once

#include <gst/gst.h>

class IPipelineFilter {

public:
	virtual ~IPipelineFilter() = default;

	/**
	 * @brief Returns the GStreamer filter bin element for this pipeline element. Has a sink and source pad.
	 * @return Pointer to the GstElement representing the filter bin.
	 */
	virtual GstElement* gstFilterBin() = 0;
};