#include "videodeviceplugin.h"

QList<SourceInfo> VideoDevicePlugin::availableSources() const
{
    QList<SourceInfo> list;
    for (const QCameraDevice& cam : QMediaDevices::videoInputs()) {
        SourceInfo info{ cam.id(), cam.description() };
        list.append(info);
    }
    return list;
}

Device* VideoDevicePlugin::createSource(const QByteArray& id, QObject* parent)
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
