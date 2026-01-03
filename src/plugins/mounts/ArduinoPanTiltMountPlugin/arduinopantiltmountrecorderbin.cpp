#include "arduinopantiltmountrecorderbin.hpp"

ArduinoPanTiltMountRecorderBin::ArduinoPanTiltMountRecorderBin(Element* element)
   : RecorderBranch(element, Source::Type::DATA)
{
    buildBodyBin();
}

bool ArduinoPanTiltMountRecorderBin::buildBodyBin()
{
    std::string deviceUuid = boost::uuids::to_string(m_uuid);

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
