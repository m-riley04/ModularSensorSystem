#include "usbaudioplugin.hpp"

std::vector<SourceInfo> USBAudioPlugin::discover() const
{ 
    return getUsbAudioDevices();
}

Source* USBAudioPlugin::createSource(const std::string& id, QObject* parent)
{
	std::vector<SourceInfo> sources = discover();
    SourceInfo selected;
    for (const SourceInfo& cam : sources) {
        if (cam.elementInfo.id == id && id != "") { // TODO: better check
            selected = cam;
			return new USBAudioSource(selected, parent);  // create the concrete source
        }
    }

    return nullptr;
}
