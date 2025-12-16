#include "controllers/elementscontroller.hpp"

ElementsController::ElementsController(SettingsController& setc, MountController& mc, SourceController& sc, ProcessingController& pc, QObject* parent)
	: QObject(parent)
	, m_sourceController(sc), m_processingController(pc), m_mountController(mc), m_settingsController(setc)
{
}

ElementsController::~ElementsController()
{
}

void ElementsController::attachSourceToMount(const QUuid& mountId, const QUuid& sourceId)
{

}

void ElementsController::detachSourceFromMount(const QUuid& mountId, const QUuid& sourceId)
{
}

void ElementsController::attachProcessorToSource(const QUuid& sourceId, const QUuid& processorId)
{
}

void ElementsController::detachProcessorFromSource(const QUuid& sourceId, const QUuid& processorId)
{
}

const QList<Source*> ElementsController::sourcesForMount(const QUuid& mountId) const
{
	QList<Source*> sources;
	const auto sourceIds = m_mountToSources.value(mountId);
	for (auto& id : sourceIds) {
		Source* source = m_sourceController.byId(id);
		sources.push_back(source);
	}
	return sources;
}

const QList<Processor*> ElementsController::processorsForSource(const QUuid& sourceId) const
{
	QList<Processor*> processors;
	const auto processorIds = m_sourceToProcessors.value(sourceId);
	for (auto& id : processorIds) {
		Processor* source = m_processingController.byId(id);
		processors.push_back(source);
	}
	return processors;
}

const QList<QUuid> ElementsController::sourceIdsForMount(const QUuid& mountId) const
{
	return m_mountToSources.value(mountId);
}

const QList<QUuid> ElementsController::processorIdsForSource(const QUuid& sourceId) const
{
	return m_sourceToProcessors.value(sourceId);
}
