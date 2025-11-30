#pragma once

#include <QString>
#include "sdk/plugins/iplugin.hpp"
#include "features/sources/source.hpp"
#include "features/processors/processor.hpp"

class Processor;

struct ProcessorInfo {
    std::string name;
    Source::Type supportedSourceType;
};

class IProcessorPlugin : public IPlugin {
public:
    virtual Source::Type supportedSourceType() const = 0;
    virtual Processor* createProcessor(Source* source, QObject* parent = nullptr) = 0;
    virtual const IElement::Type type() const override final { return IElement::Type::Processor; }
};