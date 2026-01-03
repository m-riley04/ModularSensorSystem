#include <pipeline/branches/teebranch.hpp>
#include <controllers/loggingcontroller.hpp>
#include <memory>

TeeBranch::TeeBranch(Element* element)
	: BinBase(element)
	, m_prefix(element)
	, m_body(nullptr)
{
	// Create the sink pad for the branch
	if (!makeGhostPad("sink", m_prefix.bin(), "sink")) {
		// TODO: logging
	}

	// Add the prefix bin to this bin
	if (!addMany(m_prefix.bin()))
	{
		// TODO: logging
	}

	// Link the ghost pad of this bin to the prefix bin
	if (!gst_element_link(this->bin(), m_prefix.bin()))
	{
		// TODO: logging
	}
}

TeeBranch::~TeeBranch()
{
	
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