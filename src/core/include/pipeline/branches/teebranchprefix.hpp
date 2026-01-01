#pragma once

#include <gst/gst.h>
#include <pipeline/bins/binbase.hpp>

/**
 * @brief Contains the prefix elements for a TeeBranch.
 * Currently, has a valve and a queue element.
 */
class TeeBranchPrefix : public BinBase {
private:
	GstElement* m_valve;
	GstElement* m_queue;

public:
	TeeBranchPrefix(Element*);
	virtual ~TeeBranchPrefix();

	GstElement* valve() const { return m_valve; }
	GstElement* queue() const { return m_queue; }
	bool setValveClosed(bool drop);
};
