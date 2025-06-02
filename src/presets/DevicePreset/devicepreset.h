#pragma once
#include <qstring.h>
#include <qbytearray.h>
#include <qjsonobject.h>
#include "devices/Device/device.h"

struct DevicePreset {
    QByteArray   deviceId;     // Device::id()
	QString      deviceName;   // Device::name()
    QString      pluginId;     // ISourcePlugin::pluginName()
    Device::Type sourceType;
    QJsonObject  settings;     // arbitrary, written by plugin
};