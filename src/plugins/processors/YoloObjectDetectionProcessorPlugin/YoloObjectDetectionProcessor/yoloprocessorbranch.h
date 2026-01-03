#pragma once

#include <QObject>
#include <gst/gst.h>
#include <pipeline/branches/processingbranch.hpp>
#include <QJsonDocument>

class YoloProcessorBranch : public ProcessingBranch {
public:
	YoloProcessorBranch(Element* element);
	~YoloProcessorBranch();

protected:
	virtual bool buildBodyBin() override;

private:
	GstElement* m_inference;
	GstElement* m_detector;
	GstElement* m_overlay;
};