#pragma once

#include <QObject>
#include "features/sources/source.h"
#include "USBVideoSource/usbvideosource.h"
#include <boost/dll/alias.hpp>
#include <gst/gst.h>
#include "sdk/plugins/isourceplugin.h"
#include "utils.h"

constexpr uint32_t MSS_API = 0x00010000; // 1.0.0
constexpr const char* PLUGIN_NAME = "USB Video";

class USBVideoPlugin : public ISourcePlugin
{

public:
	std::vector<SourceInfo> availableSources() const override;
	Source* createSource(const std::string& id, QObject* parent) override;
	std::string name() const override { return PLUGIN_NAME; }
	uint32_t version() const override { return MSS_API;  }
	Source::Type sourceType() const override { return Source::Type::VIDEO; }
};

static USBVideoPlugin* make_impl() { return new USBVideoPlugin(); }
static void destroy_impl(ISourcePlugin* p) { delete p; }
static uint32_t api_impl() { return MSS_API; }

BOOST_DLL_ALIAS(make_impl, mss_make)    // exports symbol "mss_make"
BOOST_DLL_ALIAS(destroy_impl, mss_destroy) // exports symbol "mss_destroy"
BOOST_DLL_ALIAS(api_impl, mss_api)     // exports symbol "mss_api"