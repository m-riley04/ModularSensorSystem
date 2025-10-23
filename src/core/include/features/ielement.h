#pragma once

#include <QObject>
#include <boost/uuid.hpp>

/**
 * An element is a generic component of the system that sends and/or recieves data.
 * Ex: mount, source, processor.
 * NOTE: I know this isn't a pure interface, but idc lol. It's convenient, it centralizes uuids, and it's close enough.
 */
class IElement : public QObject
{
	Q_OBJECT

public:
	// TODO: consider NOT using QObject at all :P
	explicit IElement(QObject* parent)
		: QObject(parent)
		, m_uuid(boost::uuids::random_generator()())
	{}
	virtual ~IElement() = default;

	/**
	 * The universally unique identifier (UUID) of the element.
	 * Assigned in the constructor.
	 * @return The UUID of the element.
	 */
	virtual boost::uuids::uuid uuid() const noexcept { return m_uuid; }

	/**
	 * The hardware/location ID of the element.
	 * @return The hardware/location ID of the element.
	 */
	virtual std::string id() const = 0;

	/**
	 * The name of the element given by the hardware/source.
	 * Not necessarily the user-facing name for the app, but is generally user-friendly to read.
	 * @return The name of the element.
	 */
	virtual std::string name() const = 0;
	virtual void setName(const std::string& newName) = 0;

	/**
	 * The ID of the plugin that created this element.
	 * By default, returns "unknown_plugin".
	 * @return string ID of the parent plugin.
	 */
	virtual std::string pluginId() const { return "unknown_plugin"; } // default implementation
	virtual void setPluginId(const std::string& newPluginId) {} // default no-op

	/**
	 * The display name of the element, used for user interfaces
	 * Can be changed by the user.
	 * By default, it displays the same as name().
	 * @return The display name of the element.
	 */
	virtual std::string displayName() const {
		return this->name();
	}
	virtual void setDisplayName(const std::string& newDisplayName) {} // default no-op

private:
	boost::uuids::uuid m_uuid;
};
