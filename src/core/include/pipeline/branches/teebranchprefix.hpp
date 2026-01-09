#pragma once

#include <gst/gst.h>
#include <pipeline/bins/binbase.hpp>
#include <core_export.hpp>

/**
 * @brief Contains the prefix elements for a TeeBranch.
 */
class MSS_CORE_API TeeBranchPrefix : public BinBase {
private:
	GstElement* m_srcQueue = nullptr;
	GstElement* m_valve = nullptr;

public:
	/**
	 * @brief Constructs a TeeBranchPrefix.
	 * @param element The parent element
	 */
	TeeBranchPrefix(Element* element);
	virtual ~TeeBranchPrefix();

	GstElement* valve() const { return m_valve; }
	bool setValveClosed(bool drop);
};
