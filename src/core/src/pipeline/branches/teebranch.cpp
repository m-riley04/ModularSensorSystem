#include <pipeline/branches/teebranch.hpp>
#include <memory>

TeeBranch::TeeBranch(boost::uuids::uuid& uuid, std::string& id) 
	: BinBase(uuid, id), m_body(nullptr), m_prefix(uuid, id)
{
	// Create the sink pad for the branch
	makeGhostPad("sink", m_prefix.bin(), "sink");
}

TeeBranch::~TeeBranch()
{
	if (m_body) {
		gst_object_unref(m_body);
		m_body = nullptr;
	}
}
