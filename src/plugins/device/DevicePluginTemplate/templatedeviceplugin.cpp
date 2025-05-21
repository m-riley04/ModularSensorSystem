#include "templatedeviceplugin.h"

QList<DeviceInfo> TemplateDevicePlugin::availableDevices() const
{
    QList<DeviceInfo> list;
    return list;
}

Device* TemplateDevicePlugin::createDevice(const QByteArray& id, QObject* parent)
{
    return nullptr;
}
