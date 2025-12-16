#pragma once

#include <QObject>
#include <QUuid>
#include "controllers/sourcecontroller.hpp"
#include "controllers/mountcontroller.hpp"
#include "controllers/processingcontroller.hpp"
#include "controllers/settingscontroller.hpp"
#include "core_export.hpp"

using OneToManyIdMap = QHash<QUuid, QList<QUuid>>;

/**
 * @brief Manages the connections between elements: mounts, sources, and processors.
 */
class MSS_CORE_API ElementsController : public QObject
{
	Q_OBJECT

public:
	ElementsController(SettingsController& setc, MountController& mc, SourceController& sc, ProcessingController& pc, QObject* parent);
	~ElementsController();

	SourceController& sourceController() const { return m_sourceController; }
	MountController& mountController() const { return m_mountController; }
	ProcessingController& processingController() const{ return m_processingController; }

	void attachSourceToMount(const QUuid& mountId, const QUuid& sourceId);
	void detachSourceFromMount(const QUuid& mountId, const QUuid& sourceId);
	void attachProcessorToSource(const QUuid& sourceId, const QUuid& processorId);
	void detachProcessorFromSource(const QUuid& sourceId, const QUuid& processorId);

	// TODO/CONSIDER: make these refs or pointers? only reason we're copying is because could be empty.
	const QList<Source*> sourcesForMount(const QUuid& mountId) const;
	const QList<Processor*> processorsForSource(const QUuid& sourceId) const;
	const QList<QUuid> sourceIdsForMount(const QUuid& mountId) const;
	const QList<QUuid> processorIdsForSource(const QUuid& sourceId) const;

private:
	SourceController& m_sourceController;
	ProcessingController& m_processingController;
	MountController& m_mountController;
	SettingsController& m_settingsController;

	OneToManyIdMap m_mountToSources;
	OneToManyIdMap m_sourceToProcessors;

};