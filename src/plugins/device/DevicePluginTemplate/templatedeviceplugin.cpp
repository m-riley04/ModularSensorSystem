#include "templatedeviceplugin.h"

QList<SourceInfo> TemplateDevicePlugin::availableSources() const
{
    QList<SourceInfo> list;
    return list;
}

Device* TemplateDevicePlugin::createSource(const QByteArray& id, QObject* parent)
{
    return nullptr;
}
