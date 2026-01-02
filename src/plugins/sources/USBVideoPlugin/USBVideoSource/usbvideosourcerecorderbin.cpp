#include "usbvideosourcerecorderbin.hpp"

USBVideoSourceRecorderBin::USBVideoSourceRecorderBin(Element* element)
   : RecorderBranch(element, Source::Type::VIDEO)
{
    buildBodyBin();
}

bool USBVideoSourceRecorderBin::buildBodyBin()
{
    std::string deviceUuid = boost::uuids::to_string(m_uuid);

    m_bin = createDefaultVideoRecordingSink(("usb_vid_recorder_sink_bin_" + deviceUuid).c_str());
    if (!m_bin) {
		LoggingController::warning("Failed to create recorder bin");
        return false;
	}

    m_encoder = gst_bin_get_by_name(GST_BIN(m_bin), "encoder");
    m_parse = gst_bin_get_by_name(GST_BIN(m_bin), "parser");
    m_muxer = gst_bin_get_by_name(GST_BIN(m_bin), "muxer");
    m_filesinkElement = gst_bin_get_by_name(GST_BIN(m_bin), "filesink");

    if (!m_encoder || !m_parse || !m_muxer || !m_filesinkElement) {
		LoggingController::warning("Failed to get one or more elements from recorder bin");
        return false;
	}

    return true;
}
