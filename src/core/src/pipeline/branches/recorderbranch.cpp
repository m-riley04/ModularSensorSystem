#include <pipeline/branches/recorderbranch.hpp>

RecorderBranch::RecorderBranch(Element* element, Source::Type sourceType)
	: TeeBranch(element)
{
	// Start branch with valve closed
	m_prefix.setValveClosed(true);
}

RecorderBranch::~RecorderBranch()
{

}
