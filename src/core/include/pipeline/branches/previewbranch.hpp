#pragma once

#include <gst/gst.h>
#include <pipeline/branches/teebranch.hpp>
#include <pipeline/branches/processingbranch.hpp>
#include <features/sources/source.hpp>
#include <core_export.hpp>

class MSS_CORE_API PreviewBranch : public TeeBranch {
protected:
	virtual bool buildBodyBin() = 0;
	ProcessingBranch* m_linkedProcessor = nullptr;

public:
	/**
	 * @brief Constructs a PreviewBranch.
	 * @param element The parent element
	 * @param enableOverlay If true, enables compositor for overlay support (for processor output)
	 */
	PreviewBranch(Element* element, bool enableOverlay = false);
	virtual ~PreviewBranch();

	/**
	 * @brief Links a processor branch to the overlay input of this preview.
	 * The processor's output will be composited on top of the source video.
	 * @param processorBranch The processor branch to link
	 * @return true if successful
	 */
	bool attachProcessor(ProcessingBranch* processorBranch);

	/**
	 * @brief Detaches the currently linked processor.
	 * @return true if successful
	 */
	bool detachProcessor();

	/**
	 * @brief Gets the currently linked processor branch.
	 * @return The linked processor branch, or nullptr if none
	 */
	ProcessingBranch* linkedProcessor() const { return m_linkedProcessor; }

	/**
	 * @brief Sets the linked processor (used when linking is done externally).
	 * @param processorBranch The processor branch that was linked
	 */
	void setLinkedProcessor(ProcessingBranch* processorBranch) { m_linkedProcessor = processorBranch; }
};