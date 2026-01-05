#pragma once

#include <pipeline/branches/previewbranch.hpp>

class TestDataSourcePreviewBranch : public PreviewBranch
{
protected:
	bool buildBodyBin() override final;

public:
	explicit TestDataSourcePreviewBranch(Element* element, bool enableOverlay = false);
	~TestDataSourcePreviewBranch() override;
};