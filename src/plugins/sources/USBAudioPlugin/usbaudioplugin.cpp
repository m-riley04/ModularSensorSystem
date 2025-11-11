#include "usbaudioplugin.hpp"

std::vector<SourceInfo> USBAudioPlugin::availableSources() const
{ 
    return getUsbAudioDevices();
}

Source* USBAudioPlugin::createSource(const std::string& id, QObject* parent)
{
	std::vector<SourceInfo> sources = availableSources();
    SourceInfo selected;
    for (const SourceInfo& cam : sources) {
        if (cam.id == id) { selected = cam; break; }
    }

	if (selected.id != "") { // TODO: better check
        return new USBAudioSource(selected, parent);  // create the concrete source
    }
    return nullptr;
}
