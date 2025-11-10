#include "usbvideosourcebin.h"

USBVideoSourceBin::USBVideoSourceBin(const boost::uuids::uuid& uuid, const std::string& id)
	: SourceBin(uuid, id, Source::Type::VIDEO, "src")
{
	build();
}

bool USBVideoSourceBin::build()
{
    std::string deviceName = boost::uuids::to_string(m_uuid);

    if (!this->create(("usb_vid_bin_" + deviceName).c_str())) return false;

    // Initialize source
    GstElement* src = gst_element_factory_make("mfvideosrc", ("usb_vid_src_" + deviceName).c_str()); // TODO: make this dynamic and cross-platform
    g_object_set(src, "device-path", m_id.c_str(), NULL); // TODO/CONSIDER: m_id should probably be better labeled to indicate it's the source id

    // Initialize queue and converter
    GstElement* conv = gst_element_factory_make("videoconvert", ("usb_vid_conv_" + deviceName).c_str());
    GstElement* queue = gst_element_factory_make("queue", ("usb_vid_queue_" + deviceName).c_str());
    //GstElement* capsf = gst_element_factory_make("capsfilter", ("usb_vid_caps_" + deviceName).c_str()); // TODO: do we need this?

    // Check validity of each
    if (!src || !conv || !queue) {
        qWarning() << "Failed to create one or more elements";
        if (src)  gst_object_unref(src);
        if (conv) gst_object_unref(conv);
        if (queue) gst_object_unref(queue);
        return false;
    }

    // Add elements to bin, and clean up if failed
    if (!this->addMany(src, conv, queue)) {
        qWarning() << "Failed to add elements to source bin";
        gst_object_unref(src);
        gst_object_unref(conv);
        gst_object_unref(queue);
        return false;
    }

    // Link elements, and clean up if failed
    if (!gst_element_link_many(src, conv, queue, NULL)) {
        qWarning() << "Failed to link mfvideosrc -> queue -> videoconvert";
        gst_bin_remove_many(GST_BIN(m_bin), src, conv, queue, NULL);
        return false;
    }

    // Create ghost source pads, and clean up if failed
    if (!this->createSrcGhostPad(queue, "src")) {
        qWarning() << "Failed to create ghost source pads";
        gst_element_unlink_many(src, conv, queue, NULL);
        gst_bin_remove_many(GST_BIN(m_bin), src, conv, queue, NULL);
        return false;
    }

    return true;
}
