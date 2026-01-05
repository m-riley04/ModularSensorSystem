#include <pipeline/branches/teebranch.hpp>
#include <controllers/loggingcontroller.hpp>
#include <memory>

TeeBranch::TeeBranch(Element* element, bool enableOverlay)
	: BinBase(element)
	, m_prefix(element, enableOverlay)
	, m_body(nullptr)
	, m_sinkPad(nullptr)
	, m_sinkOverlayPad(nullptr)
{
	// Add the prefix bin to this bin
	if (!addMany(m_prefix.bin()))
	{
		LoggingController::warning("TeeBranch: Failed to add prefix bin");
		return;
	}

	// Create a "sometimes" sink ghost pad that targets the prefix's sink pad
	m_sinkPad = makeSometimesSinkGhostPad("sink", m_prefix.bin(), "sink");
	if (!m_sinkPad) {
		LoggingController::warning("TeeBranch: Failed to create sink ghost pad");
	}

	// If overlay is enabled, create a ghost pad for the overlay input
	if (enableOverlay) {
		m_sinkOverlayPad = makeSometimesSinkGhostPad("sink_overlay", m_prefix.bin(), "sink_overlay");
		if (!m_sinkOverlayPad) {
			LoggingController::warning("TeeBranch: Failed to create sink_overlay ghost pad");
		}
	}
}

TeeBranch::~TeeBranch()
{
	// Ghost pads are owned by the bin and will be cleaned up when the bin is destroyed
	m_sinkPad = nullptr;
	m_sinkOverlayPad = nullptr;
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

bool TeeBranch::linkProcessorToOverlay(GstElement* processorBranchBin)
{
	if (!m_prefix.hasOverlay()) {
		LoggingController::warning("TeeBranch::linkProcessorToOverlay: Overlay not supported for this branch");
		return false;
	}

	if (!processorBranchBin) {
		LoggingController::warning("TeeBranch::linkProcessorToOverlay: Processor branch bin is null");
		return false;
	}

	// Get the src pad from the processor branch
	GstPad* processorSrcPad = gst_element_get_static_pad(processorBranchBin, "src");
	if (!processorSrcPad) {
		LoggingController::warning("TeeBranch::linkProcessorToOverlay: Failed to get src pad from processor branch");
		return false;
	}

	// Link to the overlay input
	bool result = m_prefix.linkPadToOverlay(processorSrcPad);
	gst_object_unref(processorSrcPad);

	return result;
}