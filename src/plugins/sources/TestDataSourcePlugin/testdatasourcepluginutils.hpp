#pragma once

#include <vector>
#include "features/sources/source.hpp"
#include <sdk/plugins/isourceplugin.hpp>

static std::vector<SourceInfo> getAvailableTestDataSources()
{
	// For testing purposes, we create a single dummy data source
	SourceInfo testDataSource{
		.elementInfo = ElementInfo{
			.id = "test_data_source_1",
			.name = "Test Data Sensor",
			.displayName = "Test Data Sensor",
			.pluginId = "test_data_source_plugin",
		},
		.type = Source::Type::DATA
	};
	return { testDataSource };
}