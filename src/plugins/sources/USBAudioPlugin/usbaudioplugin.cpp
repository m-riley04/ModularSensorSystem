#include "usbaudioplugin.hpp"

const std::vector<SourceInfo> USBAudioPlugin::discover() const
{ 
    return getUsbAudioDevices();
}

Source* USBAudioPlugin::createSource(const std::string& id, QObject* parent)
{
	const std::vector<SourceInfo> sources = discover();
    for (const SourceInfo& cam : sources) {
        if (cam.elementInfo.id == id && id != "") { // TODO: better check
			return new USBAudioSource(cam, parent);  // create the concrete source
        }
    }

    return nullptr;
}
