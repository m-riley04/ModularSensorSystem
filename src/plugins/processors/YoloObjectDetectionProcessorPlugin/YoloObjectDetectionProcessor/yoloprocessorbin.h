#pragma once

#include <QObject>
#include <gst/gst.h>
#include <pipeline/branches/processingbranch.hpp>
#include <QJsonDocument>

class YoloProcessorBin : public ProcessingBranch {
public:
	YoloProcessorBin(Element* element);
	~YoloProcessorBin();

protected:
	virtual bool buildBodyBin() override;

private:
	GstElement* m_inference;
	GstElement* m_detector;
	GstElement* m_overlay;
};