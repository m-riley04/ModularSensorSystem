#include "yoloprocessorbin.h"
#include <pipeline/filters/default_processors.hpp>
#include <utils/boost_qt_conversions.hpp>

YoloProcessorBin::YoloProcessorBin(Element* element)
	: ProcessingBranch(element)
{
	buildBodyBin();
}

YoloProcessorBin::~YoloProcessorBin()
{
	// Even though bins manage their children, we still need to unref the elements we got through gst_bin_get_by_name
	gst_object_unref(m_inference);
	gst_object_unref(m_detector);
	gst_object_unref(m_overlay);
}

bool YoloProcessorBin::buildBodyBin()
{
    std::string binName = "yolo_processor_bin";
    std::string processorUuidStr = boostUuidToQUuid(m_uuid).toString().toStdString();
    std::string binFullName = binName + "_" + processorUuidStr;

    // TODO: change this to be yolo
    m_bin = createDefaultObjectDetectorProcessorFilter(ObjectDetectorModelType::YOLO, binFullName.c_str());
    if (!m_bin) {
        LoggingController::warning("Failed to create processor filter bin");
        return false;
    }

    m_inference = gst_bin_get_by_name(GST_BIN(m_bin), "inference");
    m_detector = gst_bin_get_by_name(GST_BIN(m_bin), "tensorDecoder");
    m_overlay = gst_bin_get_by_name(GST_BIN(m_bin), "overlay");

    if (!m_inference || !m_detector || !m_overlay ) {
        LoggingController::warning("Failed to get one or more elements from processor bin");
        return false;
    }

    return true;
}
