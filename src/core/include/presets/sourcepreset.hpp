#pragma once
#include <qstring.h>
#include <qbytearray.h>
#include <qjsonobject.h>
#include "features/sources/source.hpp"

struct SourcePreset {
    QByteArray sourceId; // Device::id()
	QString sourceName; // Device::name()
    QString pluginId; // ISourcePlugin::name()
    Source::Type sourceType;
    QJsonObject settings; // arbitrary, written by plugin
};