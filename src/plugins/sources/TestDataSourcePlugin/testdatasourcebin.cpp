#include "testdatasourcebin.h"

TestDataSourceBin::TestDataSourceBin(const std::string& id)
    : SourceBin(id, Source::Type::DATA, "src")
{
	build();
}

void TestDataSourceBin::pushRandomSample(uint64_t seq, double value)
{
    if (!m_appsrc)
        return;

    // Build JSON: {"sensor_id":"random_test","seq":N,"value":X.YYYYYY}
    QByteArray json = QByteArray("{\"sensor_id\":\"")
        + QByteArray::fromStdString(m_id)
        + "\",\"seq\":" + QByteArray::number(seq)
        + ",\"value\":" + QByteArray::number(value, 'f', 6)
        + "}";

    if (!json.endsWith('\n'))
        json.append('\n');

    // Create buffer and copy payload
    GstBuffer* buf = gst_buffer_new_allocate(nullptr, json.size(), nullptr);
    GstMapInfo map;
    gst_buffer_map(buf, &map, GST_MAP_WRITE);
    memcpy(map.data, json.constData(), json.size());
    gst_buffer_unmap(buf, &map);

    // No explicit PTS/DTS: appsrc will timestamp for us
    GstFlowReturn ret = GST_FLOW_OK;
    g_signal_emit_by_name(m_appsrc, "push-buffer", buf, &ret);
    gst_buffer_unref(buf);

    // Optional: check ret for errors/EOS if you care
    // if (ret != GST_FLOW_OK) { ... }
}

bool TestDataSourceBin::build() {
    if (!this->create(("rand_data_bin_" + m_id).c_str())) {
        return false;
    }

    m_appsrc = gst_element_factory_make("appsrc", ("rand_src_" + m_id).c_str());
    GstElement* q = gst_element_factory_make("queue", ("rand_q_" + m_id).c_str());
    if (!m_appsrc || !q) return false;

    GstCaps* caps = gst_caps_new_simple("application/x-mss-random-ndjson",
        "sensor-id", G_TYPE_STRING, m_id.c_str(),
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

    if (!createSrcGhostPad(q, "src")) {
        return false;
    }

    return true;
}
