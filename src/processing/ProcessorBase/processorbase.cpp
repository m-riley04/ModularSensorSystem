#include "processorbase.h"

ProcessorBase::ProcessorBase(Device* device, QObject* parent)
	: QObject(parent), pDevice(device)
{}