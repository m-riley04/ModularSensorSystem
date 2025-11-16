#pragma once

#include <gst/gst.h>

/**
 * @brief Capability interface for elements that provide a recording sink.
 * Without implementing this interface, elements will not record any data.
 */
class IRecordableSource {
public:
	virtual ~IRecordableSource() = default;

	/**
	 * @brief Method to retrieve the custom recorder sink.
	 * @return A pointer to the custom recorder bin/sink.
	 */
	virtual GstElement* recorderSink() = 0;
};