#pragma once

#include <gst/gst.h>
#include <pipeline/bins/binbase.hpp>
#include <core_export.hpp>

/**
 * @brief Contains the prefix elements for a TeeBranch.
 * 
 * This bin has "sometimes" pads that are available when requested:
 * - sink: Main input from the source stream
 * - sink_overlay: (Optional) Input for overlay data (e.g., from processors)
 * - src: Output to the body of the branch
 * 
 * When constructed without overlay support, the bin is simply: queue -> valve
 * When constructed with overlay support: queue + overlay_queue -> compositor -> valve
 */
class MSS_CORE_API TeeBranchPrefix : public BinBase {
private:
	GstElement* m_srcQueue = nullptr;
	GstElement* m_overlayQueue = nullptr;
	GstElement* m_compositor = nullptr;
	GstElement* m_valve = nullptr;

	// Ghost pads (sometimes pads)
	GstPad* m_sinkPad = nullptr;
	GstPad* m_sinkOverlayPad = nullptr;
	GstPad* m_srcPad = nullptr;

	bool m_hasOverlay = false;

public:
	/**
	 * @brief Constructs a TeeBranchPrefix.
	 * @param element The parent element
	 * @param enableOverlay If true, creates compositor setup for overlay input. 
	 *                      If false, creates a simple queue->valve passthrough.
	 */
	TeeBranchPrefix(Element* element, bool enableOverlay = false);
	virtual ~TeeBranchPrefix();

	/**
	 * @brief Links the passed element to the overlay input of the compositor.
	 * Only valid if overlay was enabled during construction.
	 * @param overlayElem 
	 * @return 
	 */
	bool linkToOverlay(GstElement* overlayElem);

	GstElement* valve() const { return m_valve; }
	bool setValveClosed(bool drop);
	bool hasOverlay() const { return m_hasOverlay; }

	// Pad accessors
	GstPad* sinkPad() const { return m_sinkPad; }
	GstPad* sinkOverlayPad() const { return m_sinkOverlayPad; }
	GstPad* srcPad() const { return m_srcPad; }
};
