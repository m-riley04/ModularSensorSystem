#include "testdatasourcepreviewbranch.hpp"
#include <controllers/loggingcontroller.hpp>
#include <pipeline/sinks/preview_defaults.hpp>
#include <utils/boost_qt_conversions.hpp>

TestDataSourcePreviewBranch::TestDataSourcePreviewBranch(Element* element, bool enableOverlay)
	: PreviewBranch(element, enableOverlay)
{
	buildBodyBin();
}

TestDataSourcePreviewBranch::~TestDataSourcePreviewBranch()
{
}

bool TestDataSourcePreviewBranch::buildBodyBin()
{
	std::string binName = "test_data_bin";
	std::string processorUuidStr = boostUuidToQUuid(m_element->uuid()).toString().toStdString();
	std::string binFullName = binName + "_" + processorUuidStr;

	// Get window id from element
	quintptr windowId = 0;
	IPreviewable* previewableElem = m_element->asPreviewable();
	if (previewableElem) {
		windowId = previewableElem->windowId();
	}

	// Create default body
	m_body = createDefaultPreviewSink(Source::Type::DATA, windowId, binFullName.c_str());
	if (!m_body) {
		LoggingController::warning("Failed to create source body bin");
		return false;
	}

	return this->attachBody();
}