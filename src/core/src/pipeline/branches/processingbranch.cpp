#include <pipeline/branches/processingbranch.hpp>

ProcessingBranch::ProcessingBranch(Element* element)
	: TeeBranch(element)
{
}

bool ProcessingBranch::setProcessingEnabled(bool enabled) {
	return this->m_prefix.setValveClosed(!enabled);
}
