#pragma once

#include <pipeline/bins/binbase.hpp>
#include <pipeline/branches/teebranchprefix.hpp>
#include <features/element.hpp>
#include <core_export.hpp>

/**
 * @brief Represents a branch in a GStreamer tee element, managing the data flow with a controllable prefix.
 * A branch consists of elements/bins, mostly composed of:
 * 1. A prefix
 * 2. A body
 */
class MSS_CORE_API TeeBranch : public BinBase {
protected:
	TeeBranchPrefix m_prefix;
	GstElement* m_body = nullptr;

	virtual bool buildBodyBin() = 0;

public:
	TeeBranch(Element* element);
	virtual ~TeeBranch();

	GstElement* body() const { return m_body; }
	TeeBranchPrefix& prefix() { return m_prefix; }
	void setBody(GstElement* body) { m_body = body; }
	bool setValveClosed(bool closed) { return m_prefix.setValveClosed(closed); }

};