#include <pipeline/branches/teebranchprefix.hpp>
#include <controllers/loggingcontroller.hpp>

TeeBranchPrefix::TeeBranchPrefix(Element* element, bool enableOverlay)
	: BinBase(element)
	, m_hasOverlay(enableOverlay)
{
	// Create common elements
	m_srcQueue = gst_element_factory_make("queue", "srcQueue");
	m_valve = gst_element_factory_make("valve", "valve");
	
	if (!m_srcQueue || !m_valve) {
		LoggingController::warning("TeeBranchPrefix: Failed to create queue or valve element");
		return;
	}

	if (enableOverlay) {
		// Create overlay elements
		m_overlayQueue = gst_element_factory_make("queue", "overlayQueue");
		m_compositor = gst_element_factory_make("compositor", "compositor");
		
		if (!m_overlayQueue || !m_compositor) {
			LoggingController::warning("TeeBranchPrefix: Failed to create overlay elements");
			gst_object_unref(m_srcQueue);
			gst_object_unref(m_valve);
			m_srcQueue = nullptr;
			m_valve = nullptr;
			return;
		}

		// Add all elements to bin
		gst_bin_add_many(GST_BIN(m_bin), m_srcQueue, m_overlayQueue, m_compositor, m_valve, nullptr);

		// Link queues to compositor
		if (!gst_element_link(m_srcQueue, m_compositor)
			|| !gst_element_link(m_overlayQueue, m_compositor)) {
			LoggingController::warning("TeeBranchPrefix: Failed to link queues to compositor");
			return;
		}

		// Link compositor to valve
		if (!gst_element_link(m_compositor, m_valve)) {
			LoggingController::warning("TeeBranchPrefix: Failed to link compositor to valve");
			return;
		}

		// Configure compositor
		g_object_set(m_compositor, "background", 3, nullptr); // transparent background

		// Modify compositor pad properties
		GstPad* compSrcSinkPad = gst_element_get_static_pad(m_compositor, "sink_0");
		GstPad* compOverlaySinkPad = gst_element_get_static_pad(m_compositor, "sink_1");
		if (compSrcSinkPad && compOverlaySinkPad) {
			g_object_set(compSrcSinkPad, "zorder", 0, nullptr);
			g_object_set(compOverlaySinkPad, "zorder", 1, nullptr);
			gst_object_unref(compSrcSinkPad);
			gst_object_unref(compOverlaySinkPad);
		}

		// Create ghost pads - main sink, overlay sink, and src
		m_sinkPad = this->makeSometimesSinkGhostPad("sink", m_srcQueue, "sink");
		m_sinkOverlayPad = this->makeSometimesSinkGhostPad("sink_overlay", m_overlayQueue, "sink");
		m_srcPad = this->makeSometimesSrcGhostPad("src", m_valve, "src");
	}
	else {
		// Simple passthrough: queue -> valve
		gst_bin_add_many(GST_BIN(m_bin), m_srcQueue, m_valve, nullptr);

		// Link queue directly to valve
		if (!gst_element_link(m_srcQueue, m_valve)) {
			LoggingController::warning("TeeBranchPrefix: Failed to link queue to valve");
			return;
		}

		// Create ghost pads - just sink and src (no overlay)
		m_sinkPad = this->makeSometimesSinkGhostPad("sink", m_srcQueue, "sink");
		m_srcPad = this->makeSometimesSrcGhostPad("src", m_valve, "src");
	}
}

TeeBranchPrefix::~TeeBranchPrefix() {
	// Ghost pads are owned by the bin and will be cleaned up when the bin is destroyed
	m_sinkPad = nullptr;
	m_sinkOverlayPad = nullptr;
	m_srcPad = nullptr;

	// Elements are owned by the bin
	m_srcQueue = nullptr;
	m_overlayQueue = nullptr;
	m_compositor = nullptr;
	m_valve = nullptr;
}

bool TeeBranchPrefix::linkToOverlay(GstElement* overlayElem)
{
	if (!m_hasOverlay) {
		LoggingController::warning("TeeBranchPrefix::linkToOverlay: Overlay not enabled for this prefix");
		return false;
	}
	if (!overlayElem || !m_overlayQueue) return false;
	return gst_element_link(overlayElem, m_overlayQueue);
}

bool TeeBranchPrefix::setValveClosed(bool drop) {
	if (!m_valve) return false;
	g_object_set(G_OBJECT(m_valve), "drop", drop, nullptr);
	return true;
}