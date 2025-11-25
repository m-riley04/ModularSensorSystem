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
	 * @brief The file extension used for recordings from this source.
	 * @return 
	 */
	virtual std::string recorderFileExtension() const = 0;

	/**
	 * @brief Sets the file path where recordings will be saved.
	 * @param filePath The file path as a string.
	 * @return True on success, False on failure.
	 */
	virtual bool setRecordingFilePath(const std::string& filePath) = 0;

	/**
	 * @brief Opens the recording valve to start recording data.
	 * @return TRUE on success, FALSE on failure.
	 */
	virtual bool startRecording() = 0;

	/**
	 * @brief Closes the recording valve and sends EOS to stop/finalize recording data.
	 * @return TRUE on success, FALSE on failure.
	 */
	virtual bool stopRecording() = 0;
};