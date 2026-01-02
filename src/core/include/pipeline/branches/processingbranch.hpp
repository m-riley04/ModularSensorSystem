#pragma once

#include <pipeline/branches/teebranch.hpp>
#include <core_export.hpp>

class MSS_CORE_API ProcessingBranch : public TeeBranch {
protected:
	virtual bool buildBodyBin() = 0;

public:
	ProcessingBranch(Element* element);
	virtual ~ProcessingBranch() = default;
	virtual bool setProcessingEnabled(bool enabled);

};