#include "tripodmountplugin.hpp"

TripodPlugin::TripodPlugin()
{

}

TripodMount* TripodPlugin::createMount(const std::string& id, QObject* parent)
{
	for (auto& mountInfo : discover()) {
		if (mountInfo.id == id) {
			return new TripodMount(mountInfo, parent);
		}
	}
    return nullptr;
}

const std::vector<ElementInfo> TripodPlugin::discover() const
{
    std::vector<ElementInfo> list;
	ElementInfo standardTripod{
		.id = "tripod_standard",
		.name = "Standard Tripod",
		.displayName = "Standard Tripod",
		.pluginId = "tripod_mount_plugin",
	};
	list.push_back(standardTripod);

    return list;
}
