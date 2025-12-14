#include "arduinopantiltmountbin.hpp"
#include <controllers/loggingcontroller.hpp>
#include <QJsonObject>

ArduinoPanTiltMountBin::ArduinoPanTiltMountBin(const boost::uuids::uuid& uuid, const std::string& id)
	: SourceBin(uuid, id, Source::Type::DATA, "src")
{
	build();
}

void ArduinoPanTiltMountBin::pushSample(QByteArray json)
{
    if (!m_appsrc) return;

	// Convert JsonDocument to raw data
	std::string jsonStr = json.toStdString();

    // Get timestamp
	GstClockTime timestamp = gst_util_get_timestamp();

    // Construct wrapping JSON with timestamp and payload
	QJsonObject wrapperObj;
	wrapperObj.insert("timestamp", static_cast<qint64>(timestamp));
	wrapperObj.insert("payload", QJsonDocument::fromJson(json).object());
	QJsonDocument wrapperDoc(wrapperObj);

    // Create buffer and copy payload
    gsize payloadSize = jsonStr.size();
    GstBuffer* buf = gst_buffer_new_allocate(nullptr, payloadSize, nullptr);
    GstMapInfo map;
    gst_buffer_map(buf, &map, GST_MAP_WRITE);
    memcpy(map.data, &jsonStr, payloadSize);
    gst_buffer_unmap(buf, &map);

    // No explicit PTS/DTS: appsrc will timestamp for us
    GstFlowReturn ret = GST_FLOW_OK;
    g_signal_emit_by_name(m_appsrc, "push-buffer", buf, &ret);
    gst_buffer_unref(buf);
}

bool ArduinoPanTiltMountBin::build()
{
    std::string deviceName = boost::uuids::to_string(m_uuid);
    std::string gstElementPrefix = "arduino_pan_tilt";

    if (!this->create((gstElementPrefix + "_" + deviceName).c_str())) return false;
    
	// TODO: implement actual Arduino Pan-Tilt source element here

    m_appsrc = gst_element_factory_make("appsrc", (gstElementPrefix + "_src_" + deviceName).c_str());
    GstElement* q = gst_element_factory_make("queue", (gstElementPrefix + "_queue_" + deviceName).c_str());
    if (!m_appsrc || !q) return false;

	GstCaps* caps = gst_caps_new_simple("application/mss-json", // TODO: define proper mime type
        "format", G_TYPE_STRING, "string",
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
