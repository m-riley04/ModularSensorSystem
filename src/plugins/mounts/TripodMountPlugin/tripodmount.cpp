#include "tripodmount.hpp"

TripodMount::TripodMount(QObject* parent) : Mount(parent)
{
}

TripodMount::TripodMount(std::string id, std::string name, QObject* parent) : m_id(id),
	m_name(name), m_displayName(name), m_pluginId("plugin_tripod_mount"), Mount(parent)
{

}
