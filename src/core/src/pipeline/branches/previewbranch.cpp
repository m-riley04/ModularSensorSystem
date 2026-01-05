#include <pipeline/branches/previewbranch.hpp>

PreviewBranch::PreviewBranch(Element* element, bool enableOverlay)
	: TeeBranch(element, enableOverlay)
{

}

PreviewBranch::~PreviewBranch()
{
}
