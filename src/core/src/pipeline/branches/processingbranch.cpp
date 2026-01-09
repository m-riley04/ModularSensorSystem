#include <pipeline/branches/processingbranch.hpp>
#include <controllers/loggingcontroller.hpp>

ProcessingBranch::ProcessingBranch(Element* element)
	: TeeBranch(element)
	, m_srcPad(nullptr)
{
}

bool ProcessingBranch::setProcessingEnabled(bool enabled) {
	return this->m_prefix.setValveClosed(!enabled);
}

bool ProcessingBranch::createOutputPad()
{
	if (!m_body) {
		LoggingController::warning("ProcessingBranch::createOutputPad: Body is null");
		return false;
	}

	// Create a ghost pad from the body's src pad
	m_srcPad = makeGhostPad("src", m_body, "src");
	if (!m_srcPad) {
		LoggingController::warning("ProcessingBranch::createOutputPad: Failed to create src ghost pad");
		return false;
	}

	return true;
}
