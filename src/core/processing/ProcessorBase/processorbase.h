#pragma once

#include <QObject>
#include "sources/Source/source.h"
#include "interfaces/plugins/iprocessorplugin.h"

class ProcessorBase : public QObject 
{
    Q_OBJECT

public:
    ProcessorBase(Source* source, QObject* parent = nullptr);
    virtual ~ProcessorBase() = default;

    Source* source() const { return pSource; }

	void setSource(Source* source) { 
		if (source == pSource) return;
        pSource = source;
		emit sourceChanged(pSource);
    }

    virtual void startProcessing() {}
    virtual void stopProcessing() {}

protected:
    Source* pSource;

signals:
	void sourceChanged(Source*);

    // Common signals for detection events (can be extended or specialized per plugin)
    void motionDetected(Source* sourceDevice);
    void personDetected(Source* sourceDevice);
	void objectDetected(Source* sourceDevice, const QString& objectType); // TODO: Use a struct or class for object details
};
