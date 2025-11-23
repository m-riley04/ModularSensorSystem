#pragma once

#include <vector>
#include <QtMultimedia/QMediaDevices>
#include <qaudiodevice.h>
#include <sdk/plugins/isourceplugin.hpp>

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
        ElementInfo elementInfo{
            .id = mic.id().toStdString(),
            .name = mic.description().toStdString(),
            .displayName = mic.description().toStdString(),
            .pluginId = "usb_audio_plugin",
        };
        SourceInfo info{
			.elementInfo = elementInfo,
            .type = Source::Type::AUDIO
        };
        list.push_back(info);
    }
    return list;
}