#pragma once

#include <QString>
#include "sdk/plugins/iplugin.h"
#include "features/sources/source.h"
#include "features/processors/processorbase.h"

class ProcessorBase;

struct ProcessorInfo {
    std::string name;
    Source::Type supportedSourceType;
};

class IProcessorPlugin : public IPlugin {
public:
    virtual Source::Type supportedSourceType() const = 0;
    virtual ProcessorBase* createProcessor(Source* source, QObject* parent = nullptr) = 0;
    PluginType type() const override { return PluginType::Processor; }
};

#define IProcessorPlugin_iid "com.modularsensorsystem.IProcessorPlugin/1.0"
Q_DECLARE_INTERFACE(IProcessorPlugin, IProcessorPlugin_iid)