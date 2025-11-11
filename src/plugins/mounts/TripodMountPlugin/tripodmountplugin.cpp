#include "tripodmountplugin.hpp"

TripodPlugin::TripodPlugin()
{

}

TripodMount* TripodPlugin::createMount(const std::string& id, QObject* parent)
{
    if (id == "tripod_standard") {
		TripodMount* mount = new TripodMount(id, "Standard Tripod", parent);
        return mount;
	}
    return nullptr;
}

std::vector<MountInfo> TripodPlugin::discover()
{
    std::vector<MountInfo> list;

    MountInfo standardTripod;
	standardTripod.id = "tripod_standard";
	standardTripod.name = "Standard Tripod";
	list.push_back(standardTripod);

    return list;
}
