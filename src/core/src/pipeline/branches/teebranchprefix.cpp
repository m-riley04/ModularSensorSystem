#include <pipeline/branches/teebranchprefix.hpp>

TeeBranchPrefix::TeeBranchPrefix(Element* element)
	: BinBase(element)
{
	// Create elements
	m_srcQueue = gst_element_factory_make("queue", "srcQueue");
	m_overlayQueue = gst_element_factory_make("queue", "overlayQueue");
	m_compositor = gst_element_factory_make("compositor", "compositor");
	m_valve = gst_element_factory_make("valve", nullptr);
	if (!m_srcQueue || !m_overlayQueue || !m_compositor || !m_valve) {
		// TODO: logging
		return;
	}

	gst_bin_add_many(GST_BIN(m_bin), m_srcQueue, m_overlayQueue, m_compositor, m_valve, nullptr);

	// Link queues to compositor
	if (!gst_element_link(m_srcQueue, m_compositor)
		|| !gst_element_link(m_overlayQueue, m_compositor)) {
		gst_bin_remove(GST_BIN(m_bin), m_compositor);
		m_srcQueue = nullptr;
		m_overlayQueue = nullptr;
		m_compositor = nullptr;
		m_valve = nullptr;
		return;
	}

	// Link compositor to valve
	if (!gst_element_link(m_compositor, m_valve)) {
		gst_bin_remove(GST_BIN(m_bin), m_compositor);
		m_srcQueue = nullptr;
		m_overlayQueue = nullptr;
		m_compositor = nullptr;
		m_valve = nullptr;
		return;
	}

	// Properties
	g_object_set(m_compositor, "background", 3, nullptr);
	// TODO: modify pad properties as needed (z-order, operators, etc)

	// Add ghost pads
	// 2 inputs (src and overlay), 1 output (to body)
	this->makeRequestGhostPad("sink_src", m_srcQueue, "sink_src");
	this->makeRequestGhostPad("sink_overlay", m_overlayQueue, "sink_overlay");
	this->makeGhostPad("src", m_valve, "src");
}

TeeBranchPrefix::~TeeBranchPrefix() {
	// TODO/CONSIDER: proper unref and cleanup? GstBin should handle this already though
	m_srcQueue = nullptr;
	m_overlayQueue = nullptr;
	m_compositor = nullptr;
	m_valve = nullptr;
};

bool TeeBranchPrefix::linkToOverlay(GstElement* overlayElem)
{
	if (!overlayElem || !m_overlayQueue) return false;
	return gst_element_link(overlayElem, m_overlayQueue);
}

bool TeeBranchPrefix::setValveClosed(bool drop) {
	if (!m_valve) return false;
	g_object_set(G_OBJECT(m_valve), "drop", drop, nullptr);
	return true;
}