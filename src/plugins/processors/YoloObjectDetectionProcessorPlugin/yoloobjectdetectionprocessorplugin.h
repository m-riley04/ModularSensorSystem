#pragma once

#include <QObject>
#include "interfaces/plugins/iprocessorplugin.h"
#include "sources/Source/source.h"
#include "YoloObjectDetectionProcessor/yoloobjectdetectionprocessor.h"
#include <interfaces/capability/ivideosource.h>

class YoloObjectDetectionProcessor;

class YoloObjectDetectionProcessorPlugin : public QObject, public IProcessorPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID IProcessorPlugin_iid FILE "yoloobjectdetectionprocessorplugin.json")
    Q_INTERFACES(IProcessorPlugin)

public:
    Source::Type supportedSourceType() const override { return Source::Type::VIDEO; }
    QString name() const override { return "yoloobjectdetectionprocessor"; }
    ProcessorBase* createProcessor(Source* src, QObject* parent = nullptr) override
    {
        return new YoloObjectDetectionProcessor(src, parent);
    }
};
