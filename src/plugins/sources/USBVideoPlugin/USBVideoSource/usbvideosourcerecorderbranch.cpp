#include "usbvideosourcerecorderbranch.hpp"

USBVideoSourceRecorderBranch::USBVideoSourceRecorderBranch(Element* element)
   : RecorderBranch(element, Source::Type::VIDEO)
{
    buildBodyBin();
}

USBVideoSourceRecorderBranch::~USBVideoSourceRecorderBranch()
{
	// Even though bins manage their children, we still need to unref the elements we got through gst_bin_get_by_name
	if (m_encoder) gst_object_unref(m_encoder);
	if (m_parse) gst_object_unref(m_parse);
	if (m_muxer) gst_object_unref(m_muxer);
	if (m_filesinkElement) gst_object_unref(m_filesinkElement);
}

bool USBVideoSourceRecorderBranch::buildBodyBin()
{
    std::string deviceUuid = boost::uuids::to_string(m_element->uuid());

    m_body = createDefaultVideoRecordingSink(("usb_vid_recorder_sink_bin_" + deviceUuid).c_str());
    if (!m_body) {
		LoggingController::warning("Failed to create recorder bin");
        return false;
	}

    m_encoder = gst_bin_get_by_name(GST_BIN(m_body), "encoder");
    m_parse = gst_bin_get_by_name(GST_BIN(m_body), "parser");
    m_muxer = gst_bin_get_by_name(GST_BIN(m_body), "muxer");
    m_filesinkElement = gst_bin_get_by_name(GST_BIN(m_body), "filesink");

    if (!m_encoder || !m_parse || !m_muxer || !m_filesinkElement) {
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
