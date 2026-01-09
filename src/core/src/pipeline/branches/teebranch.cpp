#include <pipeline/branches/teebranch.hpp>
#include <controllers/loggingcontroller.hpp>
#include <memory>

TeeBranch::TeeBranch(Element* element)
	: BinBase(element)
	, m_prefix(element)
	, m_body(nullptr)
	, m_sinkPad(nullptr)
{
	// Add the prefix bin to this bin
	if (!addMany(m_prefix.bin()))
	{
		LoggingController::warning("TeeBranch: Failed to add prefix bin");
		return;
	}

	// Create a sink ghost pad that targets the prefix's sink pad
	m_sinkPad = makeGhostPad("sink", m_prefix.bin(), "sink");
	if (!m_sinkPad) {
		LoggingController::warning("TeeBranch: Failed to create sink ghost pad");
	}
}

TeeBranch::~TeeBranch()
{
	// Ghost pads are owned by the bin and will be cleaned up when the bin is destroyed
	m_sinkPad = nullptr;
}

bool TeeBranch::attachBody()
{
	if (!m_body) {
		LoggingController::warning("TeeBranch::attachBody: Body bin is null for element:'"
			+ QString::fromStdString(m_element->id())
			+ "'");
		return false;
	}

	// Add and link body bin to this bin
	if (!addMany(m_body))
	{
		LoggingController::warning("TeeBranch::attachBody: Failed to add body bin to branch bin for element:'"
			+ QString::fromStdString(m_element->id())
			+ "'");
		return false;
	}

	// Link the prefix bin to the body bin
	if (!gst_element_link(m_prefix.bin(), m_body))
	{
		LoggingController::warning("TeeBranch::attachBody: Failed to link prefix bin to body bin for element:'"
			+ QString::fromStdString(m_element->id())
			+ "'");
		return false;
	}

	return true;
}
