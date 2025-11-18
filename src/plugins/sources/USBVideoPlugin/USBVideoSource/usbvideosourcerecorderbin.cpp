#include "usbvideosourcerecorderbin.hpp"

USBVideoSourceRecorderBin::USBVideoSourceRecorderBin(const boost::uuids::uuid& uuid, const std::string& id)
   : RecorderBin(uuid, id, Source::Type::VIDEO, "recorder_")
{
    build();
}

bool USBVideoSourceRecorderBin::build()
{
    std::string deviceUuid = boost::uuids::to_string(m_uuid);

    if (!this->create(("usb_vid_recorder_sink_bin_" + deviceUuid).c_str())) return false;

    // Initialize elements
    m_valveElement = gst_element_factory_make("valve", ("usb_vid_valve_" + deviceUuid).c_str());
    GstElement* encoder = gst_element_factory_make("x264enc", ("usb_vid_encoder_" + deviceUuid).c_str());
    GstElement* recorder = gst_element_factory_make("filesink", ("usb_vid_writer_" + deviceUuid).c_str());
    g_object_set(m_valveElement, "drop", true, nullptr);

    // Check validity of each
    if (!m_valveElement || !encoder || !recorder) {
        qWarning() << "Failed to create one or more elements";
        if (m_valveElement)  gst_object_unref(m_valveElement);
		if (encoder) gst_object_unref(encoder);
        if (recorder) gst_object_unref(recorder);
        return false;
    }

    // Add elements to bin, and clean up if failed
    if (!this->addMany(m_valveElement, encoder, recorder)) {
        qWarning() << "Failed to add elements to source bin";
        gst_object_unref(m_valveElement);
        gst_object_unref(encoder);
        gst_object_unref(recorder);
        return false;
    }

    // Link elements, and clean up if failed
    if (!gst_element_link_many(m_valveElement, encoder, recorder, NULL)) {
        qWarning() << "Failed to link mfvideosrc -> queue -> videoconvert";
        gst_bin_remove_many(GST_BIN(m_bin), m_valveElement, encoder, recorder, NULL);
        return false;
    }

    // Create ghost sink pads, and clean up if failed
    if (!this->createSrcGhostPad(m_valveElement, "sink")) {
        qWarning() << "Failed to create ghost source pads";
        gst_element_unlink_many(m_valveElement, encoder, recorder, NULL);
        gst_bin_remove_many(GST_BIN(m_bin), m_valveElement, encoder, recorder, NULL);
        return false;
    }

    return true;
}
