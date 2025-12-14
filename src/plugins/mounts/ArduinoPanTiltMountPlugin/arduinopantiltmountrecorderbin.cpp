#include "arduinopantiltmountrecorderbin.hpp"

ArduinoPanTiltMountRecorderBin::ArduinoPanTiltMountRecorderBin(const boost::uuids::uuid& uuid, const std::string& id)
   : RecorderBin(uuid, id, Source::Type::VIDEO, "sink")
{
    build();
}

bool ArduinoPanTiltMountRecorderBin::build()
{
	// TODO: customize recorder bin for Arduino Pan-Tilt mount (csv)

    std::string deviceUuid = boost::uuids::to_string(m_uuid);

    m_bin = createDefaultVideoRecordingSink(("pan_tilt_recorder_sink_bin_" + deviceUuid).c_str());
    if (!m_bin) {
		LoggingController::warning("Failed to create recorder bin");
        return false;
	}

    m_inputQueue = gst_bin_get_by_name(GST_BIN(m_bin), "queue");
    m_valveElement = gst_bin_get_by_name(GST_BIN(m_bin), "valve");
    m_encoder = gst_bin_get_by_name(GST_BIN(m_bin), "encoder");
    m_parse = gst_bin_get_by_name(GST_BIN(m_bin), "parser");
    m_muxer = gst_bin_get_by_name(GST_BIN(m_bin), "muxer");
    m_filesinkElement = gst_bin_get_by_name(GST_BIN(m_bin), "filesink");

    if (!m_inputQueue || !m_valveElement || !m_encoder || !m_parse || !m_muxer || !m_filesinkElement) {
		LoggingController::warning("Failed to get one or more elements from recorder bin");
        return false;
	}

    return true;
}
