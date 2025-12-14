#include "arduinopantiltmountbin.hpp"
#include <controllers/loggingcontroller.hpp>

ArduinoPanTiltMountBin::ArduinoPanTiltMountBin(const boost::uuids::uuid& uuid, const std::string& id)
	: SourceBin(uuid, id, Source::Type::VIDEO, "src")
{
	build();
}

bool ArduinoPanTiltMountBin::build()
{
    std::string deviceName = boost::uuids::to_string(m_uuid);
    std::string gstElementPrefix = "arduino_pan_tilt";

    if (!this->create((gstElementPrefix + "_" + deviceName).c_str())) return false;
    
	// TODO: implement actual Arduino Pan-Tilt source element here

    // Initialize source
    GstElement* src = gst_element_factory_make("appsrc", (gstElementPrefix + "_src_" + deviceName).c_str()); // TODO: make this dynamic and cross-platform

    // Initialize queue and converter
    GstElement* queue = gst_element_factory_make("queue", (gstElementPrefix + "_queue_" + deviceName).c_str());

    // Check validity of each
    if (!src || !queue) {
        LoggingController::warning("Failed to create one or more elements");
        if (src)  gst_object_unref(src);
        if (queue) gst_object_unref(queue);
        return false;
    }

    // Add elements to bin, and clean up if failed
    if (!this->addMany(src, queue)) {
        LoggingController::warning("Failed to add elements to source bin");
        gst_object_unref(src);
        gst_object_unref(queue);
        return false;
    }

    // Link elements, and clean up if failed
    if (!gst_element_link_many(src, queue, NULL)) {
        LoggingController::warning("Failed to link appsrc -> queue");
        gst_bin_remove_many(GST_BIN(m_bin), src, queue, NULL);
        return false;
    }

    // Create ghost source pads, and clean up if failed
    if (!this->createSrcGhostPad(queue, "src")) {
        LoggingController::warning("Failed to create ghost source pads");
        gst_element_unlink_many(src, queue, NULL);
        gst_bin_remove_many(GST_BIN(m_bin), src, queue, NULL);
        return false;
    }

    return true;
}
