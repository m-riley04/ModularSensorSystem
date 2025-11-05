#include "testdatasourceplugin.h"

std::vector<SourceInfo> TestDataSourcePlugin::availableSources() const
{
	return getAvailableTestDataSources();
}

Source* TestDataSourcePlugin::createSource(const std::string& id, QObject* parent)
{
    std::vector<SourceInfo> sources = availableSources();
    SourceInfo selected;
    for (const SourceInfo& cam : sources) {
        if (cam.id == id) { selected = cam; break; }
    }

    if (selected.id != "") { // TODO: better check
        return new TestDataSource(selected, parent);  // create the concrete source
    }
    return nullptr;
}
