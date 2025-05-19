#include "videodeviceplugin.h"

QList<DeviceInfo> VideoDevicePlugin::enumerate() const
{
    QList<DeviceInfo> list;
    for (auto const& cam : QMediaDevices::videoInputs()) {
        DeviceInfo info;
        info.id = cam.id();
        info.displayName = cam.description();
        info.type = Device::CAMERA;
        list << info;
    }
    return list;
}

Device* VideoDevicePlugin::create(const QString& id, QObject* parent)
{
    for (auto const& cam : QMediaDevices::videoInputs()) {
        if (cam.id() == id.toUtf8())
            return new VideoDevice(cam, parent);  // your class
    }
    return nullptr;
}
