#pragma once

#include <QObject>
#include <gst/gst.h>
#include <pipeline/bins/filterbin.hpp>
#include <QJsonDocument>

class YoloProcessorBin : public FilterBin {
public:
	YoloProcessorBin(const boost::uuids::uuid& uuid, const std::string& id);
	~YoloProcessorBin();

	void setProcessingEnabled(bool enabled);

protected:
	virtual bool build() override;

private:
	GstElement* m_inputQueue;
	GstElement* m_inference;
	GstElement* m_detector;
	GstElement* m_overlay;
	GstElement* m_outputQueue;
};