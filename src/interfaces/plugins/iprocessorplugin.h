#pragma once

#include <QObject>
#include "sources/Source/source.h"
#include "processing/ProcessorBase/processorbase.h"

class ProcessorBase;

struct ProcessorInfo {
    QString name;
    Source::Type supportedSourceType;
};

class IProcessorPlugin {
public:
    virtual ~IProcessorPlugin() = default;
    virtual Source::Type supportedSourceType() const = 0;
    virtual QString name() const = 0;
    virtual ProcessorBase* createProcessor(Source* source, QObject* parent = nullptr) = 0;
};

#define IProcessorPlugin_iid "com.modularsensorsystem.IProcessorPlugin/1.0"
Q_DECLARE_INTERFACE(IProcessorPlugin, IProcessorPlugin_iid)