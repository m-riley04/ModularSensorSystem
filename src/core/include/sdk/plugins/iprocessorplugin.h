#pragma once

#include <QString>
#include "sdk/plugins/ielementplugin.h"
#include "features/sources/source.h"
#include "features/processors/processorbase.h"

class ProcessorBase;

struct ProcessorInfo {
    std::string name;
    Source::Type supportedSourceType;
};

class IProcessorPlugin : public IElementPlugin {
public:
    virtual Source::Type supportedSourceType() const = 0;
    virtual ProcessorBase* createProcessor(Source* source, QObject* parent = nullptr) = 0;
};

#define IProcessorPlugin_iid "com.modularsensorsystem.IProcessorPlugin/1.0"
Q_DECLARE_INTERFACE(IProcessorPlugin, IProcessorPlugin_iid)