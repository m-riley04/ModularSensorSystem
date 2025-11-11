#pragma once

#include <vector>
#include <QtMultimedia/QMediaDevices>
#include <qcameradevice.h>
#include <sdk/plugins/isourceplugin.hpp>

/**
 * Gets the list of available USB video devices.
 * @return 
 */
static std::vector<SourceInfo> getUsbVideoDevices()
{
    // TODO/CONSIDER: instead use GStreamer?
    // When I tried to, I could not get the "description"/"device-path"/"hardware id".
    // Also, it was way slower.
    std::vector<SourceInfo> list;
    for (const QCameraDevice& cam : QMediaDevices::videoInputs()) {
        SourceInfo info{ cam.id().toStdString(), cam.description().toStdString(), Source::Type::VIDEO };
        list.push_back(info);
    }
    return list;
}