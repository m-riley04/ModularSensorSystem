#pragma once

#include <vector>
#include <sdk/plugins/isourceplugin.h>
#include <QtMultimedia/QMediaDevices>
#include <qaudiodevice.h>

/**
 * Gets the list of available USB audio devices.
 * @return 
 */
static std::vector<SourceInfo> getUsbAudioDevices()
{
    // TODO/CONSIDER: instead use GStreamer?
    // When I tried to, I could not get the "description"/"device-path"/"hardware id".
    // Also, it was way slower.
    std::vector<SourceInfo> list;
    for (const QAudioDevice& mic : QMediaDevices::audioInputs()) {
        SourceInfo info{ mic.id().toStdString(), mic.description().toStdString(), Source::Type::AUDIO };
        list.push_back(info);
    }
    return list;
}