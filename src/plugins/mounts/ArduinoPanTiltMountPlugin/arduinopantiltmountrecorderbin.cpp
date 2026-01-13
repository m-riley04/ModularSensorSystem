#include "arduinopantiltmountrecorderbin.hpp"

ArduinoPanTiltMountRecorderBin::ArduinoPanTiltMountRecorderBin(Element* element)
   : RecorderBranch(element, Source::Type::DATA)
{
    buildBodyBin();
}

ArduinoPanTiltMountRecorderBin::~ArduinoPanTiltMountRecorderBin()
{
	// Bins usually own their elements. However, since we got these elements through gst_bin_get_by_name, we still must unref them.
	if (m_filesinkElement) {
        gst_object_unref(m_filesinkElement);
    }
}

bool ArduinoPanTiltMountRecorderBin::buildBodyBin()
{
    std::string deviceUuid = boost::uuids::to_string(m_element->uuid());

    m_body = createDefaultDataRecordingSink(("pan_tilt_recorder_sink_bin_" + deviceUuid).c_str());
    if (!m_body) {
		LoggingController::warning("Failed to create recorder bin");
        return false;
	}

    m_filesinkElement = gst_bin_get_by_name(GST_BIN(m_body), "filesink");

    if (!m_filesinkElement) {
		LoggingController::warning("Failed to get one or more elements from recorder bin");
        return false;
	}

    return this->attachBody();
}
