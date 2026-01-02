#pragma once

#include <pipeline/branches/teebranch.hpp>
#include <core_export.hpp>

/**
 * @brief A branch that has a composite element before the prefix.
 */
class MSS_CORE_API CompositedBranch : public TeeBranch {
private:
	GstElement* m_compositor = nullptr;

public:
	CompositedBranch(Element* element);
	virtual ~CompositedBranch();

	GstElement* compositor() const { return m_compositor; }
};