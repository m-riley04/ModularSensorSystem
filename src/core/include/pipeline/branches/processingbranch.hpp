#pragma once

#include <pipeline/branches/teebranch.hpp>
#include <core_export.hpp>

class MSS_CORE_API ProcessingBranch : public TeeBranch {
protected:
	virtual bool buildBodyBin() = 0;
	GstPad* m_srcPad = nullptr; // Output ghost pad for the processed video

	/**
	 * @brief Creates the output ghost pad for this processing branch.
	 * Should be called after attachBody() to create a "src" ghost pad from the body's output.
	 * @return true if successful
	 */
	bool createOutputPad();

public:
	ProcessingBranch(Element* element);
	virtual ~ProcessingBranch() = default;
	virtual bool setProcessingEnabled(bool enabled);

	/**
	 * @brief Gets the output pad of this processing branch.
	 * This pad can be linked to a preview branch's overlay input.
	 */
	GstPad* srcPad() const { return m_srcPad; }
};