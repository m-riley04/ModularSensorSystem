#pragma once

#include <pipeline/branches/previewbranch.hpp>

class USBAudioSourcePreviewBranch : public PreviewBranch
{
protected:
	bool buildBodyBin() override final;

public:
	explicit USBAudioSourcePreviewBranch(Element* element);
	~USBAudioSourcePreviewBranch() override;
};