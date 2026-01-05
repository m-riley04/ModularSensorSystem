#pragma once

#include <pipeline/branches/previewbranch.hpp>

class USBVideoSourcePreviewBranch : public PreviewBranch
{
protected:
	bool buildBodyBin() override final;

public:
	explicit USBVideoSourcePreviewBranch(Element* element, bool enableOverlay = false);
	~USBVideoSourcePreviewBranch() override;
};