#include "videodeviceplugin.h"

QList<DeviceInfo> VideoDevicePlugin::availableDevices() const
{
    QList<DeviceInfo> list;
    for (const QCameraDevice& cam : QMediaDevices::videoInputs()) {
        DeviceInfo info{ cam.id(), cam.description() };
        list.append(info);
    }
    return list;
}

Device* VideoDevicePlugin::createDevice(const QByteArray& id, QObject* parent)
{
    // Find the QCameraDevice by id from QMediaDevices
    QCameraDevice selected;
    for (const QCameraDevice& cam : QMediaDevices::videoInputs()) {
        if (cam.id() == id) { selected = cam; break; }
    }
    if (!selected.isNull()) {
        return new VideoDevice(selected, parent);  // create the concrete device
    }
    return nullptr;
}
