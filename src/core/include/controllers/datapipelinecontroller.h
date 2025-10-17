#pragma once

#include <QObject>
#include <gstreamer-1.0/gst/gst.h>

class DataPipelineController  : public QObject
{
	Q_OBJECT

public:
	DataPipelineController(QObject* parent);
	~DataPipelineController();
};

