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
    mDevicesProcessorsMap[device].append(proc);

    // TODO: Connect signals?

    return true;
}

void ProcessingController::unregisterAll(Device* device)
{
    // Stop and remove all processors on this device
    for (ProcessorBase* proc : mDevicesProcessorsMap[device]) {
        proc->stopProcessing();
        // Optionally disconnect signals and delete proc if owned
    }
    mDevicesProcessorsMap.remove(device);
}

QStringList ProcessingController::availableProcessorTypes()
{
	return QStringList();
}

bool ProcessingController::isCompatible(ProcessorBase* proc, Device* dev) {
    auto processorDevice = proc->device();
	if (!processorDevice) return false; // No device set in processor
    return processorDevice->deviceType() == dev->deviceType();
}