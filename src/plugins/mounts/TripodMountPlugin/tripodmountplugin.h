#pragma once

#include "sdk/plugins/imountplugin.h"

namespace {
	constexpr uint32_t MSS_API = 0x00010000; // 1.0.0
}

class TripodPlugin : public IMountPlugin
{
public:
	TripodPlugin();
	~TripodPlugin() = default;
	std::vector<Mount*> discover() override;
	std::string name() const override { return "Tripod Mount"; }
	uint32_t version() const override { return MSS_API; }
};