#include <pipeline/branches/teebranch.hpp>
#include <memory>

TeeBranch::TeeBranch(Element* element)
	: BinBase(element)
	, m_prefix(element)
	, m_body(nullptr)
{
	// Create the sink pad for the branch
	makeGhostPad("sink", m_prefix.bin(), "sink");
}

TeeBranch::~TeeBranch()
{
	if (m_body) {
		gst_object_unref(m_body);
		m_body = nullptr;
	}
}
