#pragma once
#include <qstring.h>
#include <qbytearray.h>
#include <qjsonobject.h>
#include "devices/Device/device.h"

struct DevicePreset {
    QByteArray   deviceId;     // Device::id()
    QString      pluginId;     // IDevicePlugin::pluginName()
    Device::Type deviceType;
    QJsonObject  settings;     // arbitrary, written by plugin
};