#pragma once

#include <qstring.h>
#include <qlist.h>
#include <presets/SourcePreset/sourcepreset.h>

struct Preset {
    QString                name;
	QString                path;
    QList<SourcePreset>    sources;
};