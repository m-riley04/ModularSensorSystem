#include "usbvideosourcebin.hpp"
#include <controllers/loggingcontroller.hpp>

USBVideoSourceBin::USBVideoSourceBin(Element* element)
	: SourceBin(element, Source::Type::VIDEO, "src")
{
	build();
}

bool USBVideoSourceBin::build()
{
    std::string deviceName = boost::uuids::to_string(m_element->uuid());

    if (!this->create(("usb_vid_bin_" + deviceName).c_str())) return false;

    // Initialize source
    GstElement* src = gst_element_factory_make("mfvideosrc", ("usb_vid_src_" + deviceName).c_str()); // TODO: make this dynamic and cross-platform
    g_object_set(src, "device-path", m_element->id().c_str(), NULL); // TODO/CONSIDER: m_id should probably be better labeled to indicate it's the source id

    // Initialize queue and converter
    GstElement* conv = gst_element_factory_make("videoconvert", ("usb_vid_conv_" + deviceName).c_str());
    GstElement* queue = gst_element_factory_make("queue", ("usb_vid_queue_" + deviceName).c_str());
	GstElement* capsFilter = gst_element_factory_make("capsfilter", ("usb_vid_caps_" + deviceName).c_str()); // TODO: make this dynamic based on settings

    // Check validity of each
    if (!src || !conv || !queue || !capsFilter) {
        LoggingController::warning("Failed to create one or more elements");
        if (src)  gst_object_unref(src);
        if (conv) gst_object_unref(conv);
        if (queue) gst_object_unref(queue);
        return false;
    }

    GstCaps* caps = gst_caps_new_simple(
        "video/x-raw",
        "format", G_TYPE_STRING, "NV12",
        "width", G_TYPE_INT, 1920,
        "height", G_TYPE_INT, 1080,
        "framerate", GST_TYPE_FRACTION, 30, 1,
        NULL);
    g_object_set(G_OBJECT(capsFilter), "caps", caps, nullptr);
    gst_caps_unref(caps);

    // Add elements to bin, and clean up if failed
    if (!this->addMany(src, capsFilter, conv, queue)) {
        LoggingController::warning("Failed to add elements to source bin");
        gst_object_unref(src);
        gst_object_unref(capsFilter);
        gst_object_unref(conv);
        gst_object_unref(queue);
        return false;
    }

    // Link elements, and clean up if failed
    if (!gst_element_link_many(src, capsFilter, conv, queue, NULL)) {
        LoggingController::warning("Failed to link mfvideosrc -> capsfilter -> queue -> videoconvert");
        gst_bin_remove_many(GST_BIN(m_bin), src, capsFilter, conv, queue, NULL);
        return false;
    }

    // Create ghost source pads, and clean up if failed
    if (!this->createSrcGhostPad(queue, "src")) {
        LoggingController::warning("Failed to create ghost source pads");
        gst_element_unlink_many(src, capsFilter, conv, queue, NULL);
        gst_bin_remove_many(GST_BIN(m_bin), src, capsFilter, conv, queue, NULL);
        return false;
    }

    return true;
}
