#include "controllers/datapipelinecontroller.h"

DataPipelineController::DataPipelineController(int argc, char* argv[], QObject *parent)
	: QObject(parent)
{
	// Init gstreamer
	gst_init(&argc, &argv);

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

