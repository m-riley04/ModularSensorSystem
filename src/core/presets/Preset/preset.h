#pragma once

#include <qstring.h>
#include <qlist.h>
#include <presets/DevicePreset/devicepreset.h>

struct Preset {
    QString                name;
	QString                path;
    QList<SourcePreset>    devices;
};