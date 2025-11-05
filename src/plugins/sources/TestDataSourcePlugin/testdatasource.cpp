#include "testdatasource.h"

TestDataSource::TestDataSource(const std::string& hardwareId, QObject* parent)
	: TestDataSource(getSourceInfo(hardwareId), parent)
{}

TestDataSource::TestDataSource(SourceInfo sourceInfo, QObject* parent)
	: Source(parent), m_id(sourceInfo.id), m_name(sourceInfo.displayName), m_sourceBin(std::make_unique<TestDataSourceBin>(sourceInfo.id))
{}

TestDataSource::~TestDataSource()
{}

SourceInfo TestDataSource::getSourceInfo(const std::string& id) const
{
	// Get the camera source from the id
	for (const auto& sourceInfo : getAvailableTestDataSources()) {
		if (sourceInfo.id == id) {
			return sourceInfo;
		}
	}

	// If not found, return an empty source info
	return SourceInfo();
}