#include <pipeline/branches/teebranch.hpp>
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
