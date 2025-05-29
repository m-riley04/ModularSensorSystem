#include "processingcontroller.h"

ProcessingController::ProcessingController(QObject *parent)
	: QObject(parent)
{}

ProcessingController::~ProcessingController()
{}

bool ProcessingController::registerProcessor(Device * device, ProcessorBase * proc)
{
    // Check compatibility
    if (!proc || !device) return false;
    if (!isCompatible(proc, device)) return false;
    proc->setDevice(device);         // set up signal-slot connections inside
    proc->startProcessing();
    mProcessors[device].append(proc);
    // Connect the processor's detection signals to central handler (or ClipController) if needed:
    connectSignalsForProcessor(proc);
    return true;
}

void ProcessingController::unregisterAll(Device* device)
{
    // Stop and remove all processors on this device
    for (ProcessorBase* proc : mProcessors[device]) {
        proc->stopProcessing();
        // Optionally disconnect signals and delete proc if owned
    }
    mProcessors.remove(device);
}

QStringList ProcessingController::availableProcessorTypes()
{
	return QStringList();
}

void ProcessingController::connectSignalsForProcessor(ProcessorBase* proc) {
    // If we want to route signals from processors to a global handler, we could connect here.
    // e.g., if proc has a QObject* (we can dynamic_cast to QObject to connect signals).
    // Alternatively, skip this and connect directly in attachDevice or externally.
}

bool ProcessingController::isCompatible(ProcessorBase* proc, Device* dev) {
    return proc->compatibleDeviceType() == dev->deviceType();
}