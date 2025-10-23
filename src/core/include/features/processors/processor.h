#pragma once

#include <QObject>
#include "features/sources/source.h"
#include "sdk/plugins/iprocessorplugin.h"
#include "features/ielement.h"

/**
 * An element that processes data from a source.
 * This class serves as a base for all processors that handle data from a specific source.
 */
class Processor : public IElement
{
    Q_OBJECT

public:
    Processor(Source* source, QObject* parent = nullptr) : IElement(parent), m_source(source) {}
    virtual ~Processor() = default;

    std::string id() const override {
		// TODO: implement a better ID system for processors
        return "unknown_processor";
	}

    std::string name() const override { return m_name; }
    void setName(const std::string& newName) override { m_name = newName; }
    
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

    virtual void startProcessing() {}
    virtual void stopProcessing() {}

protected:
    Source* m_source;
	std::string m_name = "Processor";

signals:
	void sourceChanged(Source*);

    // Common signals for detection events (can be extended or specialized per plugin)
    void motionDetected(Source* sourceDevice);
    void personDetected(Source* sourceDevice);
	void objectDetected(Source* sourceDevice, const QString& objectType); // TODO: Use a struct or class for object details
};
