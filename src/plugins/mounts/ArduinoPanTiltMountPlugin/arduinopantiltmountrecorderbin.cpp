#include "arduinopantiltmountrecorderbin.hpp"

ArduinoPanTiltMountRecorderBin::ArduinoPanTiltMountRecorderBin(Element* element)
   : RecorderBranch(element, Source::Type::DATA)
{
    buildBodyBin();
}

bool ArduinoPanTiltMountRecorderBin::buildBodyBin()
{
    std::string deviceUuid = boost::uuids::to_string(m_uuid);

    m_bin = createDefaultDataRecordingSink(("pan_tilt_recorder_sink_bin_" + deviceUuid).c_str());
    if (!m_bin) {
		LoggingController::warning("Failed to create recorder bin");
        return false;
	}

    m_filesinkElement = gst_bin_get_by_name(GST_BIN(m_bin), "filesink");

    if (!m_filesinkElement) {
		LoggingController::warning("Failed to get one or more elements from recorder bin");
        return false;
	}

    return true;
}
