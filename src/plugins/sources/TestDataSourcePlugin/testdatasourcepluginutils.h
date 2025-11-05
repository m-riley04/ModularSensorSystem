#pragma once

#include <vector>
#include "features/sources/source.h"
#include <sdk/plugins/isourceplugin.h>

static std::vector<SourceInfo> getAvailableTestDataSources()
{
	// For testing purposes, we create a single dummy data source
	SourceInfo testDataSource;
	testDataSource.id = "test_data_source_1";
	testDataSource.displayName = "Test Data Sensor";
	testDataSource.type = Source::Type::DATA;
	return { testDataSource };
}