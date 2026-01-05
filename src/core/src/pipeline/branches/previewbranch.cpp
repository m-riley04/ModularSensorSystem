#include <pipeline/branches/previewbranch.hpp>
#include <controllers/loggingcontroller.hpp>

PreviewBranch::PreviewBranch(Element* element, bool enableOverlay)
	: TeeBranch(element, enableOverlay)
	, m_linkedProcessor(nullptr)
{
}

PreviewBranch::~PreviewBranch()
{
	m_linkedProcessor = nullptr;
}

bool PreviewBranch::attachProcessor(ProcessingBranch* processorBranch)
{
	if (!hasOverlaySupport()) {
		LoggingController::warning("PreviewBranch::attachProcessor: This preview branch does not support overlay");
		return false;
	}

	if (!processorBranch) {
		LoggingController::warning("PreviewBranch::attachProcessor: Processor branch is null");
		return false;
	}

	if (m_linkedProcessor) {
		LoggingController::warning("PreviewBranch::attachProcessor: A processor is already attached, detach it first");
		return false;
	}

	// Link the processor's output to our overlay input
	if (!linkProcessorToOverlay(processorBranch->bin())) {
		LoggingController::warning("PreviewBranch::attachProcessor: Failed to link processor to overlay");
		return false;
	}

	m_linkedProcessor = processorBranch;
	return true;
}

bool PreviewBranch::detachProcessor()
{
	if (!m_linkedProcessor) {
		return true; // Nothing to detach
	}

	// Disable overlay (this will unlink the overlay queue from compositor)
	setOverlayEnabled(false);
	m_linkedProcessor = nullptr;
	return true;
}
