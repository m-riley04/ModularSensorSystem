#include <pipeline/branches/recorderbranch.hpp>

RecorderBranch::RecorderBranch(boost::uuids::uuid& uuid, std::string& id, Source::Type sourceType)
	: TeeBranch(uuid, id)
{

}

RecorderBranch::~RecorderBranch()
{

}
