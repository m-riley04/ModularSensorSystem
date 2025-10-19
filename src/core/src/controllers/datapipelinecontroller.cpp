#include "controllers/datapipelinecontroller.h"
#include "controllers/maincontroller.h"

DataPipelineController::DataPipelineController(MainController* mc, QObject *parent)
	: BackendControllerBase("DataPipelineController", parent), pMainController(mc), pGError(nullptr)
{
	// Init gstreamer if not already done
	if (gst_init_check(nullptr, nullptr, &pGError) != TRUE) {
		gst_init(nullptr, nullptr);
	}
	
	pPipeline = gst_pipeline_new("mss_pipeline");
}

DataPipelineController::~DataPipelineController()
{
	if (pPipeline) {
		gst_object_unref(pPipeline);
	}
	if (pMsg) {
		gst_message_unref(pMsg);
	}
}

gboolean DataPipelineController::busCallback(GstBus* bus, GstMessage* msg, gpointer data)
{
	switch (GST_MESSAGE_TYPE(msg)) {
	case GST_MESSAGE_ERROR:
		GError* err;
		gchar* debug_info;
		gst_message_parse_error(msg, &err, &debug_info);

		qDebug() << "GStreamer Error:" << err->message;
		qDebug() << "Error received from element" << GST_OBJECT_NAME(msg->src) << ":" << err->message;
		qDebug() << "Debugging information:" << (debug_info ? debug_info : "none");
		if (err) g_clear_error(&err);
		if (debug_info) g_free(debug_info);
		break;
	case GST_MESSAGE_EOS:
		qDebug() << "End-Of-Stream reached.";
		break;
	}
	return TRUE;
}

QList<const Source*> DataPipelineController::getSourcesByMount(QUuid mountId) const
{
	QList<const Source*> sources;
	const auto sourceIds = m_mountToSources.value(mountId);
	for (auto &id : sourceIds) {
		const Source* source = pMainController->sourceController()->byId(id);
		sources.push_back(source);
	}
	return sources;
}

QList<const ProcessorBase*> DataPipelineController::getProcessorsBySource(QUuid sourceId) const
{
	QList<const ProcessorBase*> processors;
	const auto processorIds = m_sourceToProcessors.value(sourceId);
	for (auto& id : processorIds) {
		const ProcessorBase* source = pMainController->processingController()->byId(id);
		processors.push_back(source);
	}
	return processors;
}

