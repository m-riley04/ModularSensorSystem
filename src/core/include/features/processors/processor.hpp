#pragma once

#include <QObject>
#include "features/sources/source.hpp"
#include "sdk/plugins/iprocessorplugin.hpp"
#include "features/element.hpp"

/**
 * An element that processes data from a source.
 * This class serves as a base for all processors that handle data from a specific source.
 */
class Processor : public Element
{
    Q_OBJECT

public:
    Processor(const ElementInfo& element, QObject* parent = nullptr) : Element(element, parent), m_source(nullptr) {}
    Processor(const ElementInfo& element, Source* source, QObject* parent = nullptr) : Element(element, parent), m_source(source) {}
    virtual ~Processor() = default;
    
    /**
     * The source that this processor is attached to.
     * @return a pointer to the source.
     */
    Source* source() const { return m_source; }
	void setSource(Source* source) { 
		if (source == m_source) return;
        m_source = source;
		emit sourceChanged(m_source);
    }

    virtual const IElement::Type elementType() const noexcept override { return IElement::Type::Processor; }

    virtual void startProcessing() {}
    virtual void stopProcessing() {}


protected:
    Source* m_source;

signals:
	void sourceChanged(Source*);

    // Common signals for detection events (can be extended or specialized per plugin)
    void motionDetected(Source* sourceDevice);
    void personDetected(Source* sourceDevice);
	void objectDetected(Source* sourceDevice, const QString& objectType); // TODO: Use a struct or class for object details
};
