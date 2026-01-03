#include "arduinopantiltmountbin.hpp"
#include <controllers/loggingcontroller.hpp>
#include <QJsonObject>
#include <gst/app/gstappsrc.h>
#include <pipeline/utils.hpp>

ArduinoPanTiltMountBin::ArduinoPanTiltMountBin(Element* element)
	: SourceBin(element, Source::Type::DATA, "src")
{
	build();
}

void ArduinoPanTiltMountBin::pushSample(QByteArray payload)
{
    if (!m_appsrc) return;
    QByteArray json = createTimestampedNdjson(payload);

    // Create buffer and copy payload
    GstBuffer* buf = gst_buffer_new_allocate(nullptr, json.size(), nullptr);

    // IMPORTANT: because gst_buffer_new_allocate() memory is not cleared,
    // clearing is a safe guard against any mismatch during refactors.
    size_t buffer_size = json.size();
    gst_buffer_memset(buf, 0, 0x00, buffer_size); // fill all bytes with 0
    gst_buffer_fill(buf, 0, json.constData(), buffer_size);

    // No explicit PTS/DTS: appsrc will timestamp for us
	// Push buffer to appsrc
    GstFlowReturn ret = gst_app_src_push_buffer(GST_APP_SRC(m_appsrc), buf);
    if (ret != GST_FLOW_OK) {
        LoggingController::warning(QString("appsrc push failed: %1").arg(ret));
    }
}

bool ArduinoPanTiltMountBin::build()
{
    std::string deviceName = boost::uuids::to_string(m_element->uuid());
    std::string gstElementPrefix = "arduino_pan_tilt";

    if (!this->create((gstElementPrefix + "_" + deviceName).c_str())) return false;
    
    m_appsrc = gst_element_factory_make("appsrc", (gstElementPrefix + "_src_" + deviceName).c_str());
    GstElement* q = gst_element_factory_make("queue", (gstElementPrefix + "_queue_" + deviceName).c_str());
    if (!m_appsrc || !q) return false;

	GstCaps* caps = gst_caps_new_simple("text/x-raw", // TODO: define proper mime type
        "format", G_TYPE_STRING, "utf8",
        nullptr);

    g_object_set(G_OBJECT(m_appsrc),
        "caps", caps,
        "is-live", TRUE,
        "format", GST_FORMAT_TIME,
        "do-timestamp", TRUE,
        nullptr);
    gst_caps_unref(caps);

    if (!this->addMany(m_appsrc, q)) {
        return false;
    }

    if (!gst_element_link(m_appsrc, q)) {
        return false;
    }

    // Expose the source ghost pad directly from the queue src pad
    if (!createSrcGhostPad(q, "src")) {
        return false;
    }

    return true;
}
