#pragma once

#include <gst/gst.h>
#include <pipeline/branches/CompositedBranch.hpp>
#include <features/sources/source.hpp>
#include <core_export.hpp>

class MSS_CORE_API PreviewBranch : public CompositedBranch {
protected:
	virtual bool buildBodyBin() = 0;

public:
	PreviewBranch(Element*);
	virtual ~PreviewBranch();
};