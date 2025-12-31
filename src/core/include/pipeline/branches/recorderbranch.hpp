#pragma once

#include <pipeline/branches/teebranch.hpp>
#include <features/sources/source.hpp>

class RecorderBranch : public TeeBranch {
private:


public:
	RecorderBranch(boost::uuids::uuid& uuid, std::string& id, Source::Type sourceType);
	virtual ~RecorderBranch();

	/**
	 * @brief Finalizes recording by sending EOS to the appropriate element(s).
	 * @return
	 */
	virtual bool finalizeRecording() = 0;
};