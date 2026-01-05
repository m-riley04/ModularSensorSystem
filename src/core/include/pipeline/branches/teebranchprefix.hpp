#pragma once

#include <gst/gst.h>
#include <pipeline/bins/binbase.hpp>
#include <core_export.hpp>

/**
 * @brief Contains the prefix elements for a TeeBranch.
 */
class MSS_CORE_API TeeBranchPrefix : public BinBase {
private:
	GstElement* m_srcQueue;
	GstElement* m_overlayQueue;
	GstElement* m_compositor;
	GstElement* m_valve;

public:
	TeeBranchPrefix(Element*);
	virtual ~TeeBranchPrefix();

	/**
	 * @brief Links the passed element to the overlay input of the compositor.
	 * @param overlayElem 
	 * @return 
	 */
	bool linkToOverlay(GstElement* overlayElem);

	GstElement* valve() const { return m_valve; }
	bool setValveClosed(bool drop);
};
