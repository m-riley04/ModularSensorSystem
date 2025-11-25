#pragma once

#include <QObject>
#include "ielement.hpp"

struct ElementInfo {
	std::string id;
	std::string name;
	std::string displayName;
	std::string pluginId;
};

/**
 * @brief The base implementation of an element.
 * Stores common/boilerplate data and functionality for all elements.
 */
class Element : public QObject, public IElement
{
	Q_OBJECT

public:
	// TODO: consider NOT using QObject at all :P
	explicit Element(QObject* parent)
		: QObject(parent)
		, m_uuid(boost::uuids::random_generator()()), m_id("unknown_id")
		, m_name("Unnamed Element"), m_pluginId("unknown_plugin")
		, m_displayName("Unnamed Element")
	{}
	Element(const std::string& id, const std::string& name, QObject* parent)
		: Element(parent)
	{
		m_id = id;
		m_name = name;
		m_displayName = name;
	}
	Element(const ElementInfo& data, QObject* parent)
		: Element(data.id, data.name, parent)
	{
		m_pluginId = data.pluginId;
		m_displayName = data.displayName;
	}
	virtual ~Element() = default;

	// Interface implementations
	virtual const boost::uuids::uuid uuid() const noexcept override final { return m_uuid; }
	virtual std::string id() const override final { return m_id; }
	virtual std::string name() const override final { return m_name; }
	virtual std::string displayName() const override final { return m_displayName; }
	virtual void setDisplayName(const std::string& newDisplayName) override final { m_displayName = newDisplayName; }
	virtual std::string pluginId() const override final { return m_pluginId; }
	

public slots:
	virtual void onSessionStart() {}; // nop by default
	virtual void onSessionStop() {}; // nop by default

protected:
	virtual void setId(const std::string& newId) override final { m_id = newId; }
	virtual void setName(const std::string& newName) override final { m_name = newName; }
	virtual void setPluginId(const std::string& newPluginId) override final { m_pluginId = newPluginId; }

private:
	boost::uuids::uuid m_uuid;
	std::string m_id;
	std::string m_name;
	std::string m_pluginId;
	std::string m_displayName;
};
