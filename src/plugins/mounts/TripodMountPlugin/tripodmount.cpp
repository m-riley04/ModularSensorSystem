#include "tripodmount.h"

TripodMount::TripodMount(QObject* parent) : Mount("tripod_default", "Default Tripod Mount", parent)
{
}

TripodMount::TripodMount(std::string id, std::string name, QObject* parent) : Mount(id, name, parent)
{}
