#pragma once

#include <QObject>
#include <quuid.h>
#include <QPointer>
#include "features/ielement.h"
#include <QUuid>
#include <gst/gst.h>

/**
 * Represents a data source (e.g., video, audio, data).
 * TODO: rename this to ISource (since I've been working on making this a more pure interface)
 */
class Source : public IElement
{
	Q_OBJECT

public:
	enum Type {
		VIDEO,
		AUDIO,
		DATA,
		OTHER
	};

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

public:
	Source(QObject* parent = nullptr) : IElement(parent) {}
	virtual ~Source() = default;

	/**
	 * The hardware ID for the device
	 * @return hardware ID string
	 */
	virtual std::string id() const override { return "src_unknown"; }

	/**
	 * The name of the device from the system.
	 * Sometimes this is called "description", other times "device-id", and even "device-path".
	 * @return user-friendly name of the device from the hardware.
	 */
	virtual std::string name() const override { return "New Source"; }
	virtual void setName(const std::string& newName) override = 0;
	virtual std::string pluginId() const override { return "plugin_unknown"; }

	/**
	 * The type of device.
	 * @return the type of a device as Source::Type enum.
	 */
	virtual Source::Type type() const { return Source::Type::OTHER; }

	/**
	 * @brief The window ID where the source's video output should be rendered.
	 * @return The window ID.
	 */
	virtual quintptr windowId() const { return 0; }
	virtual void setWindowId(quintptr newId) = 0;

	/**
	 * Gets the GStreamer bin element representing this source.
	 * @return 
	 */
	virtual GstElement* gstBin() const = 0;

};
