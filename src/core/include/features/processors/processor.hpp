#pragma once

#include <QObject>
#include "sdk/plugins/iprocessorplugin.hpp"
#include "features/element.hpp"
#include "core_export.hpp"
#include "models/processing_models.hpp"
#include <vector>

/**
 * An element that processes data from a source.
 * This class serves as a base for all processors that handle data from a specific source.
 */
class MSS_CORE_API Processor : public Element
{
    Q_OBJECT

public:
    Processor(const ElementInfo& element, QObject* parent = nullptr) : Element(element, parent) {}
    virtual ~Processor() = default;
    
    virtual bool startProcessing() = 0;
    virtual bool stopProcessing() = 0;

    // Element implementations
    virtual const IElement::Type elementType() const noexcept override { return IElement::Type::Processor; }

signals:
    void objectsDetected(std::vector<DetectionInfo>);

};
