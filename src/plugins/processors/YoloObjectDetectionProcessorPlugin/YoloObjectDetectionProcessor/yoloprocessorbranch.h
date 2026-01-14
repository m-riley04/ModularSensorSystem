#pragma once

#include <QObject>
#include <gst/gst.h>
#include <pipeline/branches/processingbranch.hpp>
#include <QJsonDocument>
#include <gst/analytics/analytics.h> // analytics meta + OD/CLS helpers
#include <glib.h>
#include <vector>
#include <gst/analytics/gstanalyticsmeta.h>

struct Detection {
	int x{}, y{}, w{}, h{};
	float confidence{};
	std::string label;
};

/**
 * @brief Extracts object detection metadata from a GStreamer buffer and converts it into a vector of Detection structures.
 * @param buffer The GStreamer buffer containing analytics metadata to extract detections from. If NULL, an empty vector is returned.
 * @return A vector of Detection objects, each containing bounding box coordinates (x, y, w, h), confidence score, and object label extracted from the buffer's analytics metadata.
 */
static std::vector<Detection> extractDetections(GstBuffer* buffer) {
    std::vector<Detection> out;
    if (!buffer) return out;

    // 1) Get analytics relation meta from the buffer (or NULL if absent)
    GstAnalyticsRelationMeta* rel = gst_buffer_get_analytics_relation_meta(buffer);  // :contentReference[oaicite:5]{index=5}
    if (!rel) return out;

    // 2) Iterate all analytics entries on this buffer
    gpointer state = nullptr;
    GstAnalyticsMtd mtd; // handle to an analytics metadata entry

    while (gst_analytics_relation_meta_iterate(
        rel, &state, GST_ANALYTICS_MTD_TYPE_ANY, &mtd)) { // :contentReference[oaicite:6]{index=6}
        const guint id = gst_analytics_mtd_get_id(&mtd);

        // 3) Try to interpret this entry as Object-Detection metadata
        GstAnalyticsODMtd od;
        if (!gst_analytics_relation_meta_get_od_mtd(rel, id, &od)) { // :contentReference[oaicite:7]{index=7}
            continue; // not an OD entry
        }

        // 4) Extract bbox + confidence + label from OD metadata
        gint x = 0, y = 0, w = 0, h = 0;
        gfloat conf = 0.0f;
        gst_analytics_od_mtd_get_location(&od, &x, &y, &w, &h, &conf); // 

        GQuark q = gst_analytics_od_mtd_get_obj_type(&od);            // 
        const char* label_c = g_quark_to_string(q);
        std::string label = label_c ? label_c : "";

        out.push_back(Detection{
          .x = x, .y = y, .w = w, .h = h,
          .confidence = conf,
          .label = std::move(label),
            });
    }

    return out;
}

class YoloProcessorBranch : public ProcessingBranch {
public:
	YoloProcessorBranch(Element* element);
	~YoloProcessorBranch();

protected:
	virtual bool buildBodyBin() override;

private:
	GstElement* m_inference;
	GstElement* m_detector;
	GstElement* m_overlay;
};