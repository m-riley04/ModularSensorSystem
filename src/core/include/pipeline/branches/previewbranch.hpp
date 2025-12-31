#pragma once

#include <gst/gst.h>
#include <pipeline/branches/teebranch.hpp>
#include <features/sources/source.hpp>

class PreviewBranch : public TeeBranch {
public:
	PreviewBranch(boost::uuids::uuid& uuid, std::string& id);
	virtual ~PreviewBranch();
};