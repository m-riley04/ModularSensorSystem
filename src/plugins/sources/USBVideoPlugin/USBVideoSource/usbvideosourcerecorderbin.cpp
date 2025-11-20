#include "usbvideosourcerecorderbin.hpp"

USBVideoSourceRecorderBin::USBVideoSourceRecorderBin(const boost::uuids::uuid& uuid, const std::string& id)
   : RecorderBin(uuid, id, Source::Type::VIDEO, "sink")
{
    build();
}

bool USBVideoSourceRecorderBin::build()
{
    std::string deviceUuid = boost::uuids::to_string(m_uuid);

    if (!this->create(("usb_vid_recorder_sink_bin_" + deviceUuid).c_str())) return false;

    // Elements: queue (from tee) -> valve -> x264enc -> h264parse -> mp4mux -> filesink
    m_inputQueue      = gst_element_factory_make("queue",     ("usb_vid_rec_queue_"   + deviceUuid).c_str());
    m_valveElement    = gst_element_factory_make("valve",     ("usb_vid_valve_"       + deviceUuid).c_str());
    m_encoder         = gst_element_factory_make("x264enc",   ("usb_vid_encoder_"     + deviceUuid).c_str());
    m_parse           = gst_element_factory_make("h264parse", ("usb_vid_parse_"       + deviceUuid).c_str());
    m_muxer           = gst_element_factory_make("mp4mux",    ("usb_vid_mp4mux_"      + deviceUuid).c_str());
    m_filesinkElement = gst_element_factory_make("filesink",  ("usb_vid_writer_"      + deviceUuid).c_str());

    // Validate elements
    if (!m_inputQueue || !m_valveElement || !m_encoder || !m_parse || !m_muxer || !m_filesinkElement) {
        qWarning() << "USBVideoSourceRecorderBin: Failed to create one or more elements";
        if (m_inputQueue)      { gst_object_unref(m_inputQueue);      m_inputQueue = nullptr; }
        if (m_valveElement)    { gst_object_unref(m_valveElement);    m_valveElement = nullptr; }
        if (m_encoder)         { gst_object_unref(m_encoder);         m_encoder = nullptr; }
        if (m_parse)           { gst_object_unref(m_parse);           m_parse = nullptr; }
        if (m_muxer)           { gst_object_unref(m_muxer);           m_muxer = nullptr; }
        if (m_filesinkElement) { gst_object_unref(m_filesinkElement); m_filesinkElement = nullptr; }
        return false;
    }

    // Close valve immediately
    g_object_set(m_valveElement, "drop", true, nullptr);

    // Reasonable defaults for live capture
    g_object_set(m_encoder,
        "tune", 0x00000004 /* zerolatency */,  // GStreamer x264enc uses flags bitfield
        nullptr);

    // Make finalized MP4 seekable earlier (optional)
    g_object_set(m_muxer, "faststart", TRUE, nullptr);

    // Make filesink async to avoid preroll
    g_object_set(m_filesinkElement, "async", FALSE, nullptr);

    // Add to bin
    if (!this->addMany(m_inputQueue, m_valveElement, m_encoder, m_parse, m_muxer, m_filesinkElement)) {
        qWarning() << "USBVideoSourceRecorderBin: Failed to add elements to bin";
        return false;
    }

    // Link chain
    if (!gst_element_link_many(m_inputQueue, m_valveElement, m_encoder, m_parse, m_muxer, m_filesinkElement, NULL)) {
        qWarning() << "USBVideoSourceRecorderBin: Failed to link queue -> valve -> encoder -> parse -> muxer -> filesink";
        gst_bin_remove_many(GST_BIN(m_bin), m_inputQueue, m_valveElement, m_encoder, m_parse, m_muxer, m_filesinkElement, NULL);
        m_inputQueue = m_valveElement = m_encoder = m_parse = m_muxer = m_filesinkElement = nullptr;
        return false;
    }

    // Create bin sink ghost pad from the input queue's sink pad
    if (!this->createSinkGhostPad(m_inputQueue, "sink")) {
        qWarning() << "USBVideoSourceRecorderBin: Failed to create sink ghost pad";
        gst_element_unlink_many(m_inputQueue, m_valveElement, m_encoder, m_parse, m_muxer, m_filesinkElement, NULL);
        gst_bin_remove_many(GST_BIN(m_bin), m_inputQueue, m_valveElement, m_encoder, m_parse, m_muxer, m_filesinkElement, NULL);
        m_inputQueue = m_valveElement = m_encoder = m_parse = m_muxer = m_filesinkElement = nullptr;
        return false;
    }

    return true;
}
