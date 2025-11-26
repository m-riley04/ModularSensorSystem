#pragma once

#include <QObject>
#include <gst/gst.h>
#include "features/sources/source.hpp"
#include "USBAudioSource/usbaudiosource.hpp"
#include <boost/dll/alias.hpp>
#include "sdk/plugins/isourceplugin.hpp"
#include "utils.hpp"

constexpr uint32_t MSS_API = 0x00010000; // 1.0.0
constexpr const char* PLUGIN_NAME = "USB Audio";

class USBAudioPlugin : public ISourcePlugin
{

public:
	const std::vector<SourceInfo> discover() const override;
	Source* createSource(const std::string& id, QObject* parent) override;
	const std::string& name() const override { return PLUGIN_NAME; }
	const uint32_t version() const override { return MSS_API;  }
	Source::Type sourceType() const override { return Source::Type::AUDIO; }

};

static USBAudioPlugin* make_impl() { return new USBAudioPlugin(); }
static void destroy_impl(ISourcePlugin* p) { delete p; }
static uint32_t api_impl() { return MSS_API; }

BOOST_DLL_ALIAS(make_impl, mss_make)    // exports symbol "mss_make"
BOOST_DLL_ALIAS(destroy_impl, mss_destroy) // exports symbol "mss_destroy"
BOOST_DLL_ALIAS(api_impl, mss_api)     // exports symbol "mss_api"