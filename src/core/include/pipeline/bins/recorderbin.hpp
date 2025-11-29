#pragma once

#include "features/sources/source.hpp"
#include "pipeline/bins/binbase.hpp"

class RecorderBin : public BinBase {
public:
	virtual ~RecorderBin() = default;

	Source::Type type() const { return m_type; }

	/**
	 * The name of the ghost pad for the source output.
	 * @return
	 */
	const char* sinkPadName() const { return m_sinkPadName.c_str(); }

	/**
	 * @brief Finalizes recording by sending EOS to the appropriate element(s).
	 * @return 
	 */
	virtual bool finalizeRecording() = 0;

protected:
	RecorderBin(const boost::uuids::uuid& uuid, const std::string& id, Source::Type sourceType, std::string sinkPadName)
		: BinBase(uuid, id),
		m_type(sourceType), m_sinkPadName(std::move(sinkPadName))
	{
	}

	virtual bool build() = 0;

	GstPad* createSinkGhostPad(GstElement* tailElement, const char* elementPadName = "sink")
	{
		return makeGhostPad(m_sinkPadName.c_str(), tailElement, elementPadName);
	}

	Source::Type m_type;
	std::string m_sinkPadName;

	GstElement* m_inputQueue = nullptr;
	GstElement* m_valveElement = nullptr;
	GstElement* m_encoder = nullptr;
	GstElement* m_filesinkElement = nullptr;

};
