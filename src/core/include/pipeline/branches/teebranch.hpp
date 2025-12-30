#pragma once

#include <pipeline/bins/binbase.hpp>
#include <pipeline/branches/teebranchprefix.hpp>

/**
 * @brief Represents a branch in a GStreamer tee element, managing the data flow with a controllable prefix.
 */
class TeeBranch : public BinBase {
private:
	TeeBranchPrefix m_prefix;
	GstElement* m_body = nullptr;

public:
	TeeBranch(boost::uuids::uuid& uuid, std::string& id);
	virtual ~TeeBranch();

	TeeBranchPrefix& prefix() { return m_prefix; }
	bool setValveClosed(bool closed) { return m_prefix.setValveClosed(closed);  }
};