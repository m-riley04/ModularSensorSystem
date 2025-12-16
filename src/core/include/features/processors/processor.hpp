#pragma once

#include <QObject>
#include "sdk/plugins/iprocessorplugin.hpp"
#include "features/element.hpp"
#include "core_export.hpp"

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
    
    /**
     * @brief Starts the processing operation.
     */
    virtual void startProcessing() = 0;

    /**
	 * @brief Stops the processing operation.
     */
    virtual void stopProcessing() = 0;

	// TODO/CONSIDER: move gst/pipeline related methods to IPipelineFilter interface?
    virtual GstElement* processorFilterBin() = 0;

    // Element implementations
    virtual const IElement::Type elementType() const noexcept override { return IElement::Type::Processor; }

};
