#include "tripodmount.h"

TripodMount::TripodMount() : Mount("tripod_default", "Default Tripod Mount")
{
}

TripodMount::TripodMount(std::string id, std::string name) : Mount(id, name)
{}
