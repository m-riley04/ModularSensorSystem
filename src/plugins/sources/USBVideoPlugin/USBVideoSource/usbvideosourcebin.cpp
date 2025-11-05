#include "usbvideosourcebin.h"

USBVideoSourceBin::USBVideoSourceBin(const std::string& id)
	: SourceBin(id, Source::Type::VIDEO, "src")
{
	build();
}

bool USBVideoSourceBin::build()
{
    if (!this->create(("usb_vid_bin_" + m_id).c_str())) return false;

    // Initialize source
    GstElement* src = gst_element_factory_make("mfvideosrc", ("usb_vid_src_" + m_id).c_str()); // TODO: make this dynamic and cross-platform
    g_object_set(src, "device-path", m_id.c_str(), NULL); // TODO/CONSIDER: m_id should probably be better labeled to indicate it's the source id

    // Initialize queue and converter
    GstElement* queue = gst_element_factory_make("queue", ("usb_vid_queue_" + m_id).c_str());
    GstElement* conv = gst_element_factory_make("videoconvert", ("usb_vid_conv_" + m_id).c_str());
    //GstElement* capsf = gst_element_factory_make("capsfilter", ("usb_vid_caps_" + m_id).c_str()); // TODO: do we need this?

    // Check validity of each
    if (!src || !queue || !conv) {
        qWarning() << "Failed to create one or more elements";
        if (src)  gst_object_unref(src);
        if (queue) gst_object_unref(queue);
        if (conv) gst_object_unref(conv);
        return false;
    }

    // Add elements to bin, and clean up if failed
    if (!this->addMany(src, queue, conv)) {
        qWarning() << "Failed to add elements to source bin";
        gst_object_unref(src);
        gst_object_unref(queue);
        gst_object_unref(conv);
        return false;
    }

    // Link elements, and clean up if failed
    if (!gst_element_link_many(src, queue, conv, NULL)) {
        qWarning() << "Failed to link mfvideosrc -> queue -> videoconvert";
        gst_bin_remove_many(GST_BIN(m_bin), src, queue, conv, NULL);
        return false;
    }

    // Create ghost source pads, and clean up if failed
    if (!this->createSrcGhostPad(conv, "src")) {
        qWarning() << "Failed to create ghost source pads";
        gst_element_unlink_many(src, queue, conv, NULL);
        gst_bin_remove_many(GST_BIN(m_bin), src, queue, conv, NULL);
        return false;
    }

    return true;
}
