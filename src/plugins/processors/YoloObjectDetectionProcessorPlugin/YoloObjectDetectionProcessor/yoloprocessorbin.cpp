#include "yoloprocessorbin.h"
#include <pipeline/filters/default_processors.hpp>
#include <utils/boost_qt_conversions.hpp>

YoloProcessorBin::YoloProcessorBin(const boost::uuids::uuid& uuid, const std::string& id)
	: FilterBin(uuid, id, "src", "sink")
{
	build();
}

bool YoloProcessorBin::build()
{
    std::string binName = "yolo_processor_bin";
    std::string processorUuidStr = boostUuidToQUuid(m_uuid).toString().toStdString();
    std::string binFullName = binName + "_" + processorUuidStr;

    m_bin = createDefaultObjectDetectorProcessorFilter(binFullName.c_str());
    if (!m_bin) {
        LoggingController::warning("Failed to create processor filter bin");
        return false;
    }

    m_inputQueue = gst_bin_get_by_name(GST_BIN(m_bin), "inputQueue");
    m_inference = gst_bin_get_by_name(GST_BIN(m_bin), "inference");
    m_detector = gst_bin_get_by_name(GST_BIN(m_bin), "detector");
    m_overlay = gst_bin_get_by_name(GST_BIN(m_bin), "overlay");
    m_outputQueue = gst_bin_get_by_name(GST_BIN(m_bin), "inputQueue");

    if (!m_inputQueue || !m_inference || !m_detector || !m_overlay || !m_outputQueue) {
        LoggingController::warning("Failed to get one or more elements from processor bin");
        return false;
    }

    return true;
}
