#include "yoloprocessorbranch.h"
#include <pipeline/filters/default_processors.hpp>
#include <controllers/loggingcontroller.hpp>
#include <utils/boost_qt_conversions.hpp>
#include <gst/app/gstappsink.h>
#include <gst/video/video.h>
#include <gst/analytics/analytics.h>
#include <gst/analytics/gstanalyticsmeta.h>

YoloProcessorBranch::YoloProcessorBranch(Element* element)
	: ProcessingBranch(element)
{
	buildBodyBin();
}

YoloProcessorBranch::~YoloProcessorBranch()
{
	// Even though bins manage their children, we still need to unref the elements we got through gst_bin_get_by_name
	if (m_inference) gst_object_unref(m_inference);
	if (m_detector) gst_object_unref(m_detector);
	if (m_overlay) gst_object_unref(m_overlay);
}

bool YoloProcessorBranch::buildBodyBin()
{
    std::string binName = "yolo_processor_bin";
    std::string processorUuidStr = boostUuidToQUuid(m_element->uuid()).toString().toStdString();
    std::string binFullName = binName + "_" + processorUuidStr;

    // TODO: change this to be yolo
    m_body = createDefaultObjectDetectorProcessorFilter(ObjectDetectorModelType::YOLO, binFullName.c_str());
    if (!m_body) {
        LoggingController::warning("Failed to create processor filter bin");
        return false;
    }

    m_inference = gst_bin_get_by_name(GST_BIN(m_body), "inference");
    m_detector = gst_bin_get_by_name(GST_BIN(m_body), "tensorDecoder");
    m_overlay = gst_bin_get_by_name(GST_BIN(m_body), "overlay");

    if (!m_inference || !m_detector || !m_overlay ) {
        LoggingController::warning("Failed to get one or more elements from processor bin");
        return false;
    }

    if (!attachBody()) {
        LoggingController::warning("Failed to attach body to processor branch");
        return false;
    }

	// Get the src pad of the body so that we can inspect buffers for detection metadata
    GstPad* pad = gst_element_get_static_pad(m_body, "src");
    gst_pad_add_probe(
        pad,
        GST_PAD_PROBE_TYPE_BUFFER,
        [](GstPad*, GstPadProbeInfo* info, gpointer userData) -> GstPadProbeReturn {
            auto* self = static_cast<YoloProcessorBranch*>(userData);
            if (!self) return GST_PAD_PROBE_OK;

            GstBuffer* buffer = gst_pad_probe_info_get_buffer(info);
            if (!buffer) return GST_PAD_PROBE_OK;

            auto detections = extractDetections(buffer);
            if (detections.empty()) return GST_PAD_PROBE_OK;

            //if (self->m_onDetections) self->m_onDetections(std::move(detections));

            return GST_PAD_PROBE_OK;
        },
        this,
        nullptr);
    gst_object_unref(pad);

	// Create output pad for linking to further branches/sinks
    if (!createOutputPad()) {
        LoggingController::warning("Failed to create output pad for processor branch");
        return false;
    }

    return true;
}
