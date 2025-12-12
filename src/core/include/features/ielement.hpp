#pragma once

#include <boost/uuid.hpp>
#include "core_export.hpp"

/**
 * An element is a generic component of the system that sends and/or recieves data.
 * Ex: mount, source, processor.
 */
class MSS_CORE_API IElement
{

public:
	enum class Type {
		Unknown = -1,
		Mount,
		Source,
		Processor
	};

public:
	virtual ~IElement() = default;

	/**
	 * The universally unique identifier (UUID) of the element.
	 * Assigned in the constructor.
	 * @return The UUID of the element.
	 */
	virtual const boost::uuids::uuid& uuid() const noexcept = 0;

	/**
	 * The hardware/location ID of the element.
	 * @return The hardware/location ID of the element.
	 */
	virtual const std::string& id() const = 0;

	/**
	 * The name of the element given by the hardware/source.
	 * Not necessarily the user-facing name for the app, but is generally user-friendly to read.
	 * @return The name of the element.
	 */
	virtual const std::string& name() const = 0;

	/**
	 * The display name of the element, used for user interfaces
	 * Can be changed by the user.
	 * By default, it displays the same as name().
	 * @return The display name of the element.
	 */
	virtual const std::string& displayName() const = 0;
	virtual void setDisplayName(const std::string& newDisplayName) = 0;

	/**
	 * The ID of the plugin that created this element.
	 * By default, returns "unknown_plugin".
	 * @return string ID of the parent plugin.
	 */
	virtual const std::string& pluginId() const = 0;

	/**
	 * @brief The type of the element.
	 * @return The type of the element.
	 */
	virtual const Type elementType() const noexcept = 0;

	/**
	 * A hook called when a session starts.
	 */
	virtual void onSessionStart() = 0;

	/**
	 * A hook called when a session stops.
	 */
	virtual void onSessionStop() = 0;

protected:
	/// These are protected to prevent external modification, but allow derived classes to set them.
	virtual void setId(const std::string& newId) = 0;
	virtual void setName(const std::string& newName) = 0;
	virtual void setPluginId(const std::string& newPluginId) = 0;

};
