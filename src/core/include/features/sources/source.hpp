#pragma once

#include <QObject>
#include "features/element.hpp"
#include "interfaces/capability/general/ipreviewable.hpp"
#include "core_export.hpp"
#include <gst/gst.h>

/**
 * Represents a data source (e.g., video, audio, data).
 * TODO: rename this to ISource (since I've been working on making this a more pure interface)
 */
class MSS_CORE_API Source : public Element
{
	Q_OBJECT

public:
	/**
	 * @brief The type of source.
	 */
	enum class Type {
		VIDEO,
		AUDIO,
		DATA,
		OTHER
	};

public:
	Source(ElementInfo& element, QObject* parent = nullptr) : Element(element, parent) {}
	virtual ~Source() = default;

	/**
	 * The type of device.
	 * @return the type of a device as Source::Type enum.
	 */
	virtual Source::Type type() const { return Source::Type::OTHER; }
	virtual const IElement::Type elementType() const noexcept override { return IElement::Type::Source; }

/// STATIC HELPERS ///
public:
	static bool isVideo(Source* src) {
		if (!src) return false;
		return src->type() == Source::Type::VIDEO;
	}

	static bool isAudio(Source* src) {
		if (!src) return false;
		return src->type() == Source::Type::AUDIO;
	}

	static bool isData(Source* src) {
		if (!src) return false;
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
		case Type::VIDEO: return "Video";
		case Type::AUDIO: return "Audio";
		case Type::DATA: return "Data";
		case Type::OTHER: return "Other";
		default: return "Unknown";
		}
	}

};
