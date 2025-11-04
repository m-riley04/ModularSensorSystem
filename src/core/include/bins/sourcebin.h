#pragma once

#include <features/sources/source.h>
#include "bins/binbase.h"

class SourceBin : public BinBase {
public:
	virtual ~SourceBin() = default;

	Source::Type type() const { return m_type; }

	/**
	 * The name of the ghost pad for the source output.
	 * @return
	 */
	const char* srcPadName() const { return m_srcPadName.c_str(); }

protected:
	SourceBin(std::string id, Source::Type sourceType, std::string srcPadName, Source* source)
		: BinBase(std::move(id)),
		m_type(sourceType), m_srcPadName(std::move(srcPadName)), m_source(source)
	{}

	virtual bool build() = 0;

	GstPad* createSrcGhostPad(GstElement* tailElement, const char* elementPadName = "src")
	{
		return makeGhostPad(m_srcPadName.c_str(), tailElement, elementPadName);
	}

	Source::Type m_type;
	std::string m_srcPadName;
	Source* m_source = nullptr;

};
