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

QList<QUuid> ElementsController::sourcesForMount(const QUuid& mountId)
{
	return m_mountToSources.value(mountId);
}

QList<QUuid> ElementsController::processorsForSource(const QUuid& sourceId)
{
	return m_sourceToProcessors.value(sourceId);
}
