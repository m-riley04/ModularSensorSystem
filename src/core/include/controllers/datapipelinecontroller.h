#pragma once

#include <QObject>
#include <gst/gst.h>
#include <QDebug>

class DataPipelineController  : public QObject
{
	Q_OBJECT

public:
	DataPipelineController(int argc, char *argv[], QObject* parent);
	~DataPipelineController();

	gboolean busCallback(GstBus* bus, GstMessage* msg, gpointer data);

private:
	GstElement* pPipeline = nullptr;
	GstMessage* pMsg = nullptr;
};

