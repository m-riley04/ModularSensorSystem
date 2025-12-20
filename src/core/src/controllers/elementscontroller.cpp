#include "controllers/elementscontroller.hpp"
#include "controllers/loggingcontroller.hpp"

ElementsController::ElementsController(SettingsController& setc, MountController& mc, SourceController& sc, ProcessingController& pc, QObject* parent)
	: QObject(parent)
	, m_sourceController(sc), m_processingController(pc), m_mountController(mc), m_settingsController(setc)
{

	// Init hash maps
	m_mountToSources = OneToManyIdMap();
	m_sourceToProcessors = OneToManyIdMap();

	// Add notifiers to clear mappings when elements are removed
	connect(&m_mountController, &MountController::mountRemoved, this, [this](const QUuid& mountId) {
		m_mountToSources.remove(mountId);
		LoggingController::info(QString("Removed all sources mapped to mount %1").arg(mountId.toString()));
		});
	connect(&m_sourceController, &SourceController::sourceRemoved, this, [this](const QUuid& sourceId) {
		// Remove sourceId from all mount mappings
		removeSourceMappings(sourceId);

		// Remove all processor mappings for this source
		m_sourceToProcessors.remove(sourceId);
		LoggingController::info(QString("Removed all processors mapped to source %1").arg(sourceId.toString()));
		});

	connect(&m_processingController, &ProcessingController::processorRemoved, this, [this](const QUuid& processorId) {
		removeProcessorMappings(processorId);
		});
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
	if (!m_sourceToProcessors.contains(sourceId)) {
		m_sourceToProcessors[sourceId] = QList<QUuid>();
	}
	m_sourceToProcessors[sourceId].append(processorId);
	LoggingController::info(QString("Attached processor %1 to source %2").arg(processorId.toString(), sourceId.toString()));
}

void ElementsController::detachProcessorFromSource(const QUuid& sourceId, const QUuid& processorId)
{
	if (m_sourceToProcessors.contains(sourceId)) {
		m_sourceToProcessors[sourceId].removeAll(processorId);
		LoggingController::info(QString("Detached processor %1 from source %2").arg(processorId.toString(), sourceId.toString()));
	} else {
		LoggingController::info(QString("Source %1 does not have processor %2 attached").arg(sourceId.toString(), processorId.toString()));
	}
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
		Processor* proc = m_processingController.byId(id);
		if (!proc) {
			LoggingController::warning(QString("Processor with ID %1 not found in ProcessingController").arg(id.toString()));
			continue;
		}
		processors.push_back(proc);
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

void ElementsController::removeSourceMappings(const QUuid& sourceId) {
	// Remove sourceId from all mount mappings
	for (auto& mountId : m_mountToSources.keys()) {
		m_mountToSources[mountId].removeAll(sourceId);
	}
	LoggingController::info(QString("Removed source %1 from all mount mappings").arg(sourceId.toString()));
}

void ElementsController::removeProcessorMappings(const QUuid& processorId) {
	// Remove processorId from all source mappings
	for (auto& sourceId : m_sourceToProcessors.keys()) {
		m_sourceToProcessors[sourceId].removeAll(processorId);
	}
	LoggingController::info(QString("Removed processor %1 from all source mappings").arg(processorId.toString()));
}