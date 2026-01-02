#pragma once

#include "pipeline/bins/binbase.hpp"

class FilterBin : public BinBase {
public:
	virtual ~FilterBin() = default;

	/**
	 * The name of the ghost pad for the source output.
	 * @return
	 */
	const char* srcPadName() const { return m_srcPadName.c_str(); }

protected:
	FilterBin(Element* element, std::string srcPadName, std::string sinkPadName)
		: BinBase(element)
		, m_srcPadName(std::move(srcPadName)), m_sinkPadName(std::move(sinkPadName))
	{
	}

	virtual bool build() = 0;

	GstPad* createSrcGhostPad(GstElement* tailElement, const char* elementPadName = "src")
	{
		return makeGhostPad(m_srcPadName.c_str(), tailElement, elementPadName);
	}

	GstPad* createSinkGhostPad(GstElement* tailElement, const char* elementPadName = "sink")
	{
		return makeGhostPad(m_sinkPadName.c_str(), tailElement, elementPadName);
	}

	std::string m_srcPadName;
	std::string m_sinkPadName;

	GstElement* m_valveElement = nullptr;

};
