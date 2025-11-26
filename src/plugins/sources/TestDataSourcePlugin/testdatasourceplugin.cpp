#include "testdatasourceplugin.hpp"

const std::vector<SourceInfo> TestDataSourcePlugin::discover() const
{
	return getAvailableTestDataSources();
}

Source* TestDataSourcePlugin::createSource(const std::string& id, QObject* parent)
{
    std::vector<SourceInfo> sources = discover();
    SourceInfo selected;
    for (const SourceInfo& cam : sources) {
        if (cam.elementInfo.id == id && selected.elementInfo.id != "") {
			return new TestDataSource(cam, parent);
        }
    }
    return nullptr;
}
