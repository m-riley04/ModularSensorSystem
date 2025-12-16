#include "controllers/processingcontroller.hpp"

ProcessingController::ProcessingController(QObject *parent)
	: QObject(parent)
{}

ProcessingController::~ProcessingController()
{
	mProcessors.clear();
	mProcessorsById.clear();
}

Processor* ProcessingController::byId(const QUuid& id) const {
	return mProcessorsById.value(id);
}

void ProcessingController::addProcessor(IProcessorPlugin* plugin)
{
	if (!plugin) return; // Ensure valid pointers
	auto processor = plugin->createProcessor(this); // TODO/CONSIDER: Pass a valid Device pointer if needed?
	if (!processor) return; // Failed to create processor
	mProcessors.append(processor);
	emit processorAdded(processor);
}

void ProcessingController::removeProcessor(Processor* processor)
{
	if (!processor) return;
	
	QUuid processorId = boostUuidToQUuid(processor->uuid());
	
	// Remove from lists/maps
	mProcessors.removeAll(processor);
	mProcessorsById.remove(processorId);
	
	emit processorRemoved(processorId);

	// Schedule the processor for deletion (safe, deferred deletion)
	processor->deleteLater();
}

void ProcessingController::clearProcessors()
{
	QList<QPointer<Processor>> processorsCopy = mProcessors; // Corrected variable name for clarity
	for (auto& processorPtr : processorsCopy) {
		removeProcessor(processorPtr);
	}
	mProcessors.clear();
	mProcessorsById.clear();
}

bool ProcessingController::isCompatible(Processor* proc, Source* src) {
	// TOOD: implement later
	//auto processorSource = proc->source();
	//if (!processorSource) return false; // No source set in processor
	//return processorSource->type() == src->type();
	return false;
}