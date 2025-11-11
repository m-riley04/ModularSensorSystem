#include "usbvideoplugin.hpp"

std::vector<SourceInfo> USBVideoPlugin::availableSources() const
{ 
    return getUsbVideoDevices();
}

Source* USBVideoPlugin::createSource(const std::string& id, QObject* parent)
{
	std::vector<SourceInfo> sources = availableSources();
    SourceInfo selected;
    for (const SourceInfo& cam : sources) {
        if (cam.id == id) { selected = cam; break; }
    }

	if (selected.id != "") { // TODO: better check
        return new USBVideoSource(selected, parent);  // create the concrete source
    }
    return nullptr;
}
