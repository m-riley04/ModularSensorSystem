#include "usbaudiosourcebin.h"

USBAudioSourceBin::USBAudioSourceBin(const boost::uuids::uuid& uuid, const std::string& id)
	: SourceBin(uuid, id, Source::Type::AUDIO, "src")
{
	build();
}

bool USBAudioSourceBin::build()
{
    std::string deviceName = boost::uuids::to_string(m_uuid);

    if (!this->create(("usb_aud_bin_" + deviceName).c_str())) return false;

    // Initialize source
    // TODO: make this dynamic and cross-platform
    GstElement* src = gst_element_factory_make("wasapi2src", ("usb_aud_src_" + deviceName).c_str()); 
    g_object_set(src, "device", m_id.c_str(), NULL);
	g_object_set(src, "low-latency", TRUE, NULL); // TODO: make sure this doesn't affect anything

    // Initialize queue and converter
    GstElement* conv = gst_element_factory_make("audioconvert", ("usb_aud_conv_" + deviceName).c_str());
    GstElement* resample = gst_element_factory_make("audioresample", ("usb_aud_resample_" + deviceName).c_str());
    GstElement* queue = gst_element_factory_make("queue", ("usb_aud_queue_" + deviceName).c_str());
    //GstElement* capsf = gst_element_factory_make("capsfilter", ("usb_aud_caps_" + deviceName).c_str()); // TODO: do we need this?

    // Check validity of each
    if (!src || !conv || !resample || !queue) {
        qWarning() << "Failed to create one or more elements";
        if (src)  gst_object_unref(src);
        if (conv) gst_object_unref(conv);
		if (resample) gst_object_unref(resample);
        if (queue) gst_object_unref(queue);
        return false;
    }

    // Add elements to bin, and clean up if failed
    if (!this->addMany(src, conv, resample, queue)) {
        qWarning() << "Failed to add elements to source bin";
        gst_object_unref(src);
        gst_object_unref(conv);
		gst_object_unref(resample);
        gst_object_unref(queue);
        return false;
    }

    // Link elements, and clean up if failed
    if (!gst_element_link_many(src, conv, resample, queue, NULL)) {
        qWarning() << "Failed to link wasapisrc2 -> conv -> resample -> queue";
        gst_bin_remove_many(GST_BIN(m_bin), src, conv, resample, queue, NULL);
        return false;
    }

    // Create ghost source pads, and clean up if failed
    if (!this->createSrcGhostPad(queue, "src")) {
        qWarning() << "Failed to create ghost source pads";
        gst_element_unlink_many(src, conv, resample, queue, NULL);
        gst_bin_remove_many(GST_BIN(m_bin), src, conv, resample, queue, NULL);
        return false;
    }

    return true;
}
