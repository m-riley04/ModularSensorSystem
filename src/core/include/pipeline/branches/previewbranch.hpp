#pragma once

#include <gst/gst.h>
#include <pipeline/branches/CompositedBranch.hpp>
#include <features/sources/source.hpp>

class PreviewBranch : public CompositedBranch {
public:
	PreviewBranch(Element*);
	virtual ~PreviewBranch();
};