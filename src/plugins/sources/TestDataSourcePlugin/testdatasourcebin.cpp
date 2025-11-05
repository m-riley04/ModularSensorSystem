#include "testdatasourcebin.h"

TestDataSourceBin::TestDataSourceBin(const std::string& id)
    : SourceBin(id, Source::Type::DATA, "src"), m_sessionPipeline(nullptr)
{
	build();
}

void TestDataSourceBin::pushRandomSample(uint64_t seq, double value)
{
    if (!m_appsrc || !m_sessionPipeline) return;

    // Get running-time PTS for this session
    GstClock* clock = gst_pipeline_get_clock(GST_PIPELINE(m_sessionPipeline));
    GstClockTime now = gst_clock_get_time(clock);
    gst_object_unref(clock);
    GstClockTime base = gst_element_get_base_time(m_sessionPipeline);
    GstClockTime pts = now - base;

    // Build JSON
    QByteArray json = QByteArray("{\"sensor_id\":\"") +
        QByteArray::fromStdString(m_id) +
        "\",\"seq\":" + QByteArray::number(seq) +
        ",\"value\":" + QByteArray::number(value, 'f', 6) +
        "}";
    if (!json.endsWith('\n')) json.append('\n');

    GstBuffer* buf = gst_buffer_new_allocate(nullptr, json.size(), nullptr);
    GstMapInfo map;
    gst_buffer_map(buf, &map, GST_MAP_WRITE);
    memcpy(map.data, json.constData(), json.size());
    gst_buffer_unmap(buf, &map);

    GST_BUFFER_PTS(buf) = pts;
    GST_BUFFER_DTS(buf) = GST_CLOCK_TIME_NONE;

    GstFlowReturn ret;
    g_signal_emit_by_name(m_appsrc, "push-buffer", buf, &ret);
    gst_buffer_unref(buf);
    // you can log / handle ret if needed
}

bool TestDataSourceBin::build()
{
    if (!this->create(("rand_data_bin_" + m_id).c_str()))
        return false;

    m_appsrc = gst_element_factory_make("appsrc", ("rand_src_" + m_id).c_str());
    GstElement* q = gst_element_factory_make("queue", ("rand_q_" + m_id).c_str());
    if (!m_appsrc || !q) return false;

    // Describe this as NDJSON telemetry
    GstCaps* caps = gst_caps_new_simple("application/x-mss-random-ndjson",
        "sensor-id", G_TYPE_STRING, m_id.c_str(),
        // “rate” is just nominal; real interval is variable
        "rate", GST_TYPE_FRACTION, 1, 1,
        nullptr);

    g_object_set(G_OBJECT(m_appsrc),
        "caps", caps,
        "is-live", TRUE,
        "format", GST_FORMAT_TIME,
        nullptr);
    gst_caps_unref(caps);

    if (!this->addMany(m_appsrc, q)) {
        return false;
    }

    if (!gst_element_link(m_appsrc, q)) {
        return false;
    }

    // Expose src_data ghost pad from queue’s src pad
    if (!createSrcGhostPad(q, "src")) {
        return false;
    }

    return true;
    
}
