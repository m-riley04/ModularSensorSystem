#pragma once

#include <gst/gst.h>

/**
 * @brief Capability interface for elements that provide a preview.
 * Without overriding this interface, elements will use a default preview mechanism.
 */
class IPreviewableSource {
public:
	virtual ~IPreviewableSource() = default;

	/**
	 * @brief Method to retrieve the preview bin sink.
	 * @return A pointer or reference to the custom preview sink bin.
	 */
	virtual GstElement* previewSinkBin() = 0;

	/**
	 * @brief Retrieves the sink bin's element name.
	 * @return The element name as a string.
	 */
	virtual std::string previewSinkElementName() const = 0;

	/**
	 * @brief The window ID where the source's preview should be rendered.
	 * @return The window ID.
	 */
	virtual guintptr windowId() const = 0;
	virtual void setWindowId(guintptr newId) = 0;
};