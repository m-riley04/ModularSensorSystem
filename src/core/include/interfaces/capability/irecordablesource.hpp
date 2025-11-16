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
	virtual GstElement* recorderSinkBin() = 0;

	/**
	 * @brief Retrieves the sink bin's element name.
	 * @return The element name as a string.
	 */
	virtual std::string recorderSinkElementName() const = 0;

	/**
	 * @brief Opens the recording valve to start recording data.
	 * @return TRUE on success, FALSE on failure.
	 */
	virtual gboolean openRecordingValve() = 0;

	/**
	 * @brief Closes the recording valve to stop recording data.
	 * @return TRUE on success, FALSE on failure.
	 */
	virtual gboolean closeRecordingValve() = 0;
};