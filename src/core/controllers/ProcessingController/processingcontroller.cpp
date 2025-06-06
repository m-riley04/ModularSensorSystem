#include "processingcontroller.h"

ProcessingController::ProcessingController(QObject *parent)
	: BackendControllerBase("ProcessingController", parent)
{}

ProcessingController::~ProcessingController()
{}

void ProcessingController::addProcessor(IProcessorPlugin* plugin)
{
	if (!plugin) return; // Ensure valid pointers
	auto processor = plugin->createProcessor(nullptr, this); // TODO/CONSIDER: Pass a valid Device pointer if needed?
	if (!processor) return; // Failed to create processor
	mProcessors.append(processor);
	emit processorAdded(processor);
}

void ProcessingController::removeProcessor(ProcessorBase* processor)
{

	emit processorRemoved(processor);
}

bool ProcessingController::isCompatible(ProcessorBase* proc, Source* src) {
    auto processorSource = proc->source();
	if (!processorSource) return false; // No source set in processor
    return processorSource->type() == src->type();
}