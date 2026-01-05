#pragma once

#include <gst/gst.h>
#include <pipeline/branches/teebranch.hpp>
#include <features/sources/source.hpp>
#include <core_export.hpp>

class MSS_CORE_API PreviewBranch : public TeeBranch {
protected:
	virtual bool buildBodyBin() = 0;

public:
	/**
	 * @brief Constructs a PreviewBranch.
	 * @param element The parent element
	 * @param enableOverlay If true, enables compositor for overlay support
	 */
	PreviewBranch(Element* element, bool enableOverlay = false);
	virtual ~PreviewBranch();
};