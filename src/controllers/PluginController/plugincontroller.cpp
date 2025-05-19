#include "plugincontroller.h"

PluginManager::PluginManager(const QString& root, QObject* parent)
    : QObject(parent)
{
    QDir dir(root + "/devices");
    for (auto file : dir.entryList(QDir::Files)) {
        QPluginLoader loader(dir.filePath(file));
        if (auto instance = loader.instance()) {
            if (auto dp = qobject_cast<IDevicePlugin*>(instance)) {
                mDevicePlugins << dp;
            }
            else {
                loader.unload();
            }
        }
    }
}