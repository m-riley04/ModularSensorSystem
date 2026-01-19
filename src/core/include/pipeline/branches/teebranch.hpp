#pragma once

#include <pipeline/bins/binbase.hpp>
#include <pipeline/branches/teebranchprefix.hpp>
#include <features/element.hpp>
#include <core_export.hpp>

/**
 * @brief Represents a branch in a GStreamer tee element, managing the data flow with a controllable prefix.
 * A branch consists of elements/bins, mostly composed of:
 * 1. A prefix
 * 2. A body
 * A "branch" is really just a bin with a specific structure.
 */
class MSS_CORE_API TeeBranch : public BinBase {
protected:
	TeeBranchPrefix m_prefix;
	GstElement* m_body = nullptr;
	GstPad* m_sinkPad = nullptr;

	/**
	 * @brief Builds the body bin for this branch.
	 * @return 
	 */
	virtual bool buildBodyBin() = 0;

	/**
	 * @brief Adds and links the body bin to this branch bin.
	 * @return 
	 */
	virtual bool attachBody();

public:
	/**
	 * @brief Constructs a TeeBranch.
	 * @param element The parent element
	 */
	TeeBranch(Element* element);
	virtual ~TeeBranch();

	GstElement* body() const { return m_body; }
	TeeBranchPrefix& prefix() { return m_prefix; }
	GstPad* sinkPad() const { return m_sinkPad; }
	void setBody(GstElement* body) { m_body = body; }
	bool setValveClosed(bool closed) { return m_prefix.setValveClosed(closed); }
};