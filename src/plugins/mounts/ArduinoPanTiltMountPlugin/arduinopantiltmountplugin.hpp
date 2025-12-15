#pragma once

#include <boost/dll/alias.hpp>
#include "sdk/plugins/imountplugin.hpp"
#include "arduinopantiltmount.hpp"
#include <QDebug>

namespace {
	constexpr uint32_t MSS_API = 0x00010000; // 1.0.0
}

class ArduinoPanTiltMountPlugin : public IMountPlugin
{
public:
	ArduinoPanTiltMountPlugin();
	~ArduinoPanTiltMountPlugin() = default;
	ArduinoPanTiltMount* createMount(const std::string& id, QObject* parent) override;
	const std::vector<ElementInfo> discover() const override;
	const std::string name() const override { return "Arduino Pan Tilt Mount"; }
	const uint32_t version() const override { return MSS_API; }
	const std::string description() const override { return "A custom pan-tilt mount using 2 servos and an Arduino Uno."; }
	const std::string author() const override { return "Riley Meyerkorth"; }
	const bool isCore() const override { return true; }
};

// Export proper factory functions that create/destroy the plugin instance
static IMountPlugin* make_impl() { return new ArduinoPanTiltMountPlugin(); } // NOTE TO SELF: Make sure this ABI uses the correct class name. Otherwise, heap corruption may occur. For example, use IMountPlugin instead of IPlugin.
static void destroy_impl(IMountPlugin* p) { delete p; }
static uint32_t api_impl() { return MSS_API; }

BOOST_DLL_ALIAS(make_impl, mss_make)    // exports symbol "mss_make"
BOOST_DLL_ALIAS(destroy_impl, mss_destroy) // exports symbol "mss_destroy"
BOOST_DLL_ALIAS(api_impl, mss_api)     // exports symbol "mss_api"