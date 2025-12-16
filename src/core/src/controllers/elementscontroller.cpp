#include "controllers/elementscontroller.hpp"
#include "controllers/loggingcontroller.hpp"

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
	m_mountToSources.value(mountId).append(sourceId);
	LoggingController::info(QString("Attached source %1 to mount %2").arg(sourceId.toString(), mountId.toString()));
}

void ElementsController::detachSourceFromMount(const QUuid& mountId, const QUuid& sourceId)
{
	m_mountToSources.value(mountId).removeAll(sourceId);
	LoggingController::info(QString("Detached source %1 from mount %2").arg(sourceId.toString(), mountId.toString()));
}

void ElementsController::attachProcessorToSource(const QUuid& sourceId, const QUuid& processorId)
{
	m_sourceToProcessors.value(sourceId).append(processorId);
	LoggingController::info(QString("Attached processor %1 to source %2").arg(processorId.toString(), sourceId.toString()));
}

void ElementsController::detachProcessorFromSource(const QUuid& sourceId, const QUuid& processorId)
{
	m_sourceToProcessors.value(sourceId).removeAll(processorId);
	LoggingController::info(QString("Detached processor %1 from source %2").arg(processorId.toString(), sourceId.toString()));
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
