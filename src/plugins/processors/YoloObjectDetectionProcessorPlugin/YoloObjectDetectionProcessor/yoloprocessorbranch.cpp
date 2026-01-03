#include "yoloprocessorbranch.h"
#include <pipeline/filters/default_processors.hpp>
#include <utils/boost_qt_conversions.hpp>

YoloProcessorBranch::YoloProcessorBranch(Element* element)
	: ProcessingBranch(element)
{
	buildBodyBin();
}

YoloProcessorBranch::~YoloProcessorBranch()
{
	// Even though bins manage their children, we still need to unref the elements we got through gst_bin_get_by_name
	gst_object_unref(m_inference);
	gst_object_unref(m_detector);
	gst_object_unref(m_overlay);
}

bool YoloProcessorBranch::buildBodyBin()
{
    std::string binName = "yolo_processor_bin";
    std::string processorUuidStr = boostUuidToQUuid(m_uuid).toString().toStdString();
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

    // Add and link body bin to this bin
    if (!addMany(m_body))
    {
        LoggingController::warning("Failed to add recorder body bin to recorder branch bin");
        return false;
    }

    // Link the prefix bin to the body bin
    if (!gst_element_link(m_prefix.bin(), m_body))
    {
        LoggingController::warning("Failed to link recorder branch prefix bin to body bin");
        return false;
    }

    return true;
}
