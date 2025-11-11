#pragma once

#include <boost/dll/alias.hpp>
#include "sdk/plugins/imountplugin.hpp"
#include "tripodmount.hpp"

namespace {
	constexpr uint32_t MSS_API = 0x00010000; // 1.0.0
}

class TripodPlugin : public IMountPlugin
{
public:
	TripodPlugin();
	~TripodPlugin() = default;
	TripodMount* createMount(const std::string& id, QObject* parent) override;
	std::vector<MountInfo> discover() override;
	std::string name() const override { return "Tripod Mount"; }
	uint32_t version() const override { return MSS_API; }
};

// Export proper factory functions that create/destroy the plugin instance
static IPlugin* make_impl() { return new TripodPlugin(); }
static void destroy_impl(IPlugin* p) { delete p; }
static uint32_t api_impl() { return MSS_API; }

BOOST_DLL_ALIAS(make_impl, mss_make)    // exports symbol "mss_make"
BOOST_DLL_ALIAS(destroy_impl, mss_destroy) // exports symbol "mss_destroy"
BOOST_DLL_ALIAS(api_impl, mss_api)     // exports symbol "mss_api"