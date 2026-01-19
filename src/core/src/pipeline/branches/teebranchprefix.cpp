#include <pipeline/branches/teebranchprefix.hpp>
#include <controllers/loggingcontroller.hpp>

TeeBranchPrefix::TeeBranchPrefix(Element* element)
	: BinBase(element)
{
	// Create common elements
	m_srcQueue = gst_element_factory_make("queue", "srcQueue");
	m_valve = gst_element_factory_make("valve", "valve");
	if (!m_srcQueue || !m_valve) {
		LoggingController::warning("TeeBranchPrefix: Failed to create queue or valve element");
		return;
	}
	
	// Simple passthrough: queue -> valve
	gst_bin_add_many(GST_BIN(m_bin), m_srcQueue, m_valve, nullptr);

	// Link queue directly to valve
	if (!gst_element_link(m_srcQueue, m_valve)) {
		LoggingController::warning("TeeBranchPrefix: Failed to link queue to valve");
		return;
	}

	// Create ghost pads - just sink and src (no overlay)
	this->makeGhostPad("sink", m_srcQueue, "sink");
	this->makeGhostPad("src", m_valve, "src");
}

TeeBranchPrefix::~TeeBranchPrefix() {
	// Elements are owned by the bin
	m_srcQueue = nullptr;
	m_valve = nullptr;
}

bool TeeBranchPrefix::setValveClosed(bool drop) {
	if (!m_valve) return false;
	g_object_set(G_OBJECT(m_valve), "drop", drop, nullptr);
	return true;
}