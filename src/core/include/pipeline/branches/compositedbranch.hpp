#pragma once

#include <pipeline/branches/teebranch.hpp>

/**
 * @brief A branch that has a composite element before the prefix.
 */
class CompositedBranch : public TeeBranch {
private:
	GstElement* m_compositor = nullptr;

public:
	CompositedBranch(boost::uuids::uuid& uuid, std::string& id);
	virtual ~CompositedBranch();

	GstElement* compositor() const { return m_compositor; }
};