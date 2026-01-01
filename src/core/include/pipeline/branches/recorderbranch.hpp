#pragma once

#include <pipeline/branches/teebranch.hpp>
#include <features/sources/source.hpp>

class RecorderBranch : public TeeBranch {
private:


public:
	RecorderBranch(Element* element, Source::Type sourceType);
	virtual ~RecorderBranch();

	/**
	 * @brief Finalizes recording by sending EOS to the appropriate element(s).
	 * @return
	 */
	virtual bool finalizeRecording() = 0;
};