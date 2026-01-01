#pragma once

#include <pipeline/bins/binbase.hpp>
#include <pipeline/branches/teebranchprefix.hpp>
#include <features/element.hpp>

/**
 * @brief Represents a branch in a GStreamer tee element, managing the data flow with a controllable prefix.
 * A branch consists of elements/bins.
 */
class TeeBranch : public BinBase {
private:
	TeeBranchPrefix m_prefix;
	GstElement* m_body = nullptr;

public:
	TeeBranch(Element* element);
	virtual ~TeeBranch();

	GstElement* body() const { return m_body; }
	TeeBranchPrefix& prefix() { return m_prefix; }
	void setBody(GstElement* body) { m_body = body; }
	bool setValveClosed(bool closed) { return m_prefix.setValveClosed(closed); }

};