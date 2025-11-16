#pragma once

#include <QObject>
#include "features/ielement.hpp"
#include "interfaces/capability/ipreviewablesource.hpp"
#include "interfaces/capability/irecordablesource.hpp"
#include <gst/gst.h>

/**
 * Represents a data source (e.g., video, audio, data).
 * TODO: rename this to ISource (since I've been working on making this a more pure interface)
 */
class Source : public IElement
{
	Q_OBJECT

public:
	/**
	 * @brief The type of source.
	 */
	enum Type {
		VIDEO,
		AUDIO,
		DATA,
		OTHER
	};

public:
	Source(QObject* parent = nullptr) : IElement(parent) {}
	virtual ~Source() = default;

	/**
	 * Gets the GStreamer bin element representing this source.
	 * @return
	 */
	virtual GstElement* srcBin() = 0;

	/// CAPABILITY INTERFACES ///

	virtual IPreviewableSource* asPreviewable() { return dynamic_cast<IPreviewableSource*>(this); }
	virtual IPreviewableSource* asRecordable() { return dynamic_cast<IPreviewableSource*>(this); }

	/// OVERRIDES ///

	/**
	 * The hardware ID for the device.
	 * Different APIs have different names for this, including:
	 * - "id" (QtMultimedia)
	 * - "device_name" (FFmpeg)
	 * - "device-name" (GStreamer)
	 * @return hardware ID string
	 */
	virtual std::string id() const override = 0;

	/**
	 * The name of the device from the system.
	 * Different APIs have different names for this, including:
	 * - "description" (QtMultimedia)
	 * - "device_description" (FFmpeg)
	 * - "device-id" or "device-path" (GStreamer)
	 * @return user-friendly name of the device from the hardware.
	 */
	virtual std::string name() const override = 0;
	virtual void setName(const std::string& newName) override = 0;
	virtual std::string pluginId() const override = 0;

	/**
	 * The type of device.
	 * @return the type of a device as Source::Type enum.
	 */
	virtual Source::Type type() const { return Source::Type::OTHER; }

/// STATIC HELPERS ///
public:
	static bool isVideo(Source* src) {
		return src->type() == Source::Type::VIDEO;
	}

	static bool isAudio(Source* src) {
		return src->type() == Source::Type::AUDIO;
	}

	static bool isData(Source* src) {
		return src->type() == Source::Type::DATA;
	}

	/**
	 * Converts a Type enum value to its corresponding string representation.
	 * @param type The type to convert.
	 * @return The string representation of the type.
	 */
	static std::string typeToString(Type type)
	{
		switch (type) {
		case VIDEO: return "Video";
		case AUDIO: return "Audio";
		case DATA: return "Data";
		case OTHER: return "Other";
		default: return "Unknown";
		}
	}

};
