#pragma once

#include <QObject>
#include "sdk/plugins/iprocessorplugin.hpp"
#include "features/sources/source.hpp"
#include "interfaces/capability/ivideosource.hpp"
#include <boost/dll/alias.hpp>

namespace {
    constexpr uint32_t MSS_API = 0x00010000; // 1.0.0
}

class YoloObjectDetectionProcessorPlugin : public IProcessorPlugin {

public:
    Source::Type supportedSourceType() const override { return Source::Type::VIDEO; }
    const std::string name() const override { return "YOLO Object Detection"; }
    const uint32_t version() const override { return 0x00010000; }
    Processor* createProcessor(Source* src, QObject* parent = nullptr) override;
};

static YoloObjectDetectionProcessorPlugin* make_impl() { return new YoloObjectDetectionProcessorPlugin(); }
static void destroy_impl(IProcessorPlugin* p) { delete p; }
static uint32_t api_impl() { return MSS_API; }

BOOST_DLL_ALIAS(make_impl, mss_make)    // exports symbol "mss_make"
BOOST_DLL_ALIAS(destroy_impl, mss_destroy) // exports symbol "mss_destroy"
BOOST_DLL_ALIAS(api_impl, mss_api)     // exports symbol "mss_api"