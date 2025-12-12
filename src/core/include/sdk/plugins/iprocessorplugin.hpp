#pragma once

#include <QString>
#include "sdk/plugins/iplugin.hpp"
#include "features/sources/source.hpp"
#include "features/processors/processor.hpp"
#include "core_export.hpp"

class Processor;

// Note: ProcessorInfo doesn't need MSS_CORE_API - it's a simple POD struct
// defined entirely in the header with no out-of-line member functions.
struct ProcessorInfo {
    std::string name;
    Source::Type supportedSourceType;
};

class MSS_CORE_API IProcessorPlugin : public IPlugin {
public:
    virtual Source::Type supportedSourceType() const = 0;
    virtual Processor* createProcessor(Source* source, QObject* parent = nullptr) = 0;
    virtual const IElement::Type type() const override final { return IElement::Type::Processor; }
};