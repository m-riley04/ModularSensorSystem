#include <pipeline/branches/teebranchprefix.hpp>

TeeBranchPrefix::TeeBranchPrefix(Element* element)
	: BinBase(element)
{
	m_valve = gst_element_factory_make("valve", nullptr);
	m_queue = gst_element_factory_make("queue", nullptr);

	if (!m_valve || !m_queue) {
		// TODO: Proper error handling
	}

	gst_bin_add_many(GST_BIN(m_bin), m_valve, m_queue, nullptr);

	if (!gst_element_link(m_valve, m_queue)) {
		// TODO: proper error handling
	}

	// Create source and sink pads
	this->makeGhostPad("src", m_valve, "src");
	this->makeGhostPad("sink", m_queue, "sink");
}

TeeBranchPrefix::~TeeBranchPrefix() {
	// TODO/CONSIDER: proper unref and cleanup? GstBin should handle this already though

	m_bin = nullptr;
	m_valve = nullptr;
	m_queue = nullptr;
};

bool TeeBranchPrefix::setValveClosed(bool drop) {
	if (!m_valve) return false;
	g_object_set(G_OBJECT(m_valve), "drop", drop, nullptr);
	return true;
}