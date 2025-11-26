#include "usbvideoplugin.hpp"

const std::vector<SourceInfo> USBVideoPlugin::discover() const
{ 
    return getUsbVideoDevices();
}

Source* USBVideoPlugin::createSource(const std::string& id, QObject* parent)
{
	std::vector<SourceInfo> sources = discover();
    for (const SourceInfo& cam : sources) {
        if (cam.elementInfo.id == id && cam.elementInfo.id != "") {
			return new USBVideoSource(cam, parent);
        }
    }
    return nullptr;
}
