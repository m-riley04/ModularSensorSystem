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
	bool m_overlayEnabled = false;

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
	 * @brief Links the passed element's src pad to the overlay input of the compositor.
	 * Only valid if overlay was enabled during construction.
	 * @param overlayElem The element to link (its "src" pad will be linked)
	 * @return true if successful
	 */
	bool linkToOverlay(GstElement* overlayElem);

	/**
	 * @brief Links a specific pad to the overlay input.
	 * @param srcPad The source pad to link to the overlay input
	 * @return true if successful
	 */
	bool linkPadToOverlay(GstPad* srcPad);

	/**
	 * @brief Enables or disables the overlay input.
	 * When disabled, only the main source input is composited.
	 * @param enabled Whether to enable the overlay
	 * @return true if successful
	 */
	bool setOverlayEnabled(bool enabled);
	bool isOverlayEnabled() const { return m_overlayEnabled; }

	GstElement* valve() const { return m_valve; }
	GstElement* compositor() const { return m_compositor; }
	GstElement* overlayQueue() const { return m_overlayQueue; }
	bool setValveClosed(bool drop);
	bool hasOverlay() const { return m_hasOverlay; }

	// Pad accessors
	GstPad* sinkPad() const { return m_sinkPad; }
	GstPad* sinkOverlayPad() const { return m_sinkOverlayPad; }
	GstPad* srcPad() const { return m_srcPad; }
};
