#pragma once

#include <QObject>
#include <gst/gst.h>
#include <QDebug>
#include "controllers/backendcontrollerbase.h"
#include <QList>
#include <QHash>
#include <QUuid>
#include <vector>

class MainController;
class Source;
class ProcessorBase;

using OneToManyIdMap = QHash<QUuid, std::vector<QUuid>>;

class DataPipelineController : public BackendControllerBase
{
	Q_OBJECT

public:
	DataPipelineController(MainController*, QObject* parent);
	~DataPipelineController();

	gboolean busCallback(GstBus* bus, GstMessage* msg, gpointer data);

	QList<const Source*> getSourcesByMount(QUuid mountId) const;
	QList<const ProcessorBase*> getProcessorsBySource(QUuid sourceId) const;

private:
	GError* pGError = nullptr;
	GstElement* pPipeline = nullptr;
	GstMessage* pMsg = nullptr;

	OneToManyIdMap m_mountToSources;
	OneToManyIdMap m_sourceToProcessors;

	MainController* pMainController = nullptr;
};

