#pragma once

#include <QObject>
#include <gst/gst.h>
#include <pipeline/branches/processingbranch.hpp>
#include <QJsonDocument>
#include <gst/analytics/analytics.h> // analytics meta + OD/CLS helpers
#include <glib.h>
#include <vector>
#include <gst/analytics/gstanalyticsmeta.h>
#include <functional>
#include "interfaces/capability/processors/iobjectdetectioncapable.hpp"

/**
 * @brief Extracts object detection metadata from a GStreamer buffer and converts it into a vector of Detection structures.
 * @param buffer The GStreamer buffer containing analytics metadata to extract detections from. Can be null (to get an empty vector).
 * @return A vector of Detection objects.
 */
static std::vector<DetectionInfo> extractDetections(GstBuffer* buffer) {
    std::vector<DetectionInfo> out;
    if (!buffer) return out;

    // Get analytics relation meta from the buffer (or NULL if absent)
    GstAnalyticsRelationMeta* rel = gst_buffer_get_analytics_relation_meta(buffer); // :contentReference[oaicite:5]{index=5}
    if (!rel) return out;

    // Iterate all analytics entries on this buffer
    gpointer state = nullptr;
    GstAnalyticsMtd mtd; // handle to an analytics metadata entry

    while (gst_analytics_relation_meta_iterate(
        rel, &state, GST_ANALYTICS_MTD_TYPE_ANY, &mtd)) {
        const guint id = gst_analytics_mtd_get_id(&mtd);

        // Try to interpret this entry as Object-Detection metadata
        GstAnalyticsODMtd od;
        if (!gst_analytics_relation_meta_get_od_mtd(rel, id, &od)) {
            continue; // not an OD entry
        }

        // Extract bbox + confidence + label from OD metadata
        gint x = 0, y = 0, w = 0, h = 0;
        gfloat conf = 0.0f;
        gst_analytics_od_mtd_get_location(&od, &x, &y, &w, &h, &conf);

        GQuark q = gst_analytics_od_mtd_get_obj_type(&od);
        const char* label_c = g_quark_to_string(q);
        std::string label = label_c ? label_c : "";

        out.push_back(DetectionInfo{
          .label = std::move(label.data()),
		  .labelLength = label.size(),
          .confidence = conf,
          .x = x, .y = y, .width = w, .height = h,
            });
    }

    return out;
}

class YoloProcessorBranch : public ProcessingBranch {
public:
	using DetectionsCallback = std::function<void(std::vector<DetectionInfo>)>;

	YoloProcessorBranch(Element* element);
	~YoloProcessorBranch();

	void setDetectionsCallback(DetectionsCallback cb) { m_onDetections = std::move(cb); }

protected:
	virtual bool buildBodyBin() override;

private:
	DetectionsCallback m_onDetections;
	GstElement* m_inference;
	GstElement* m_detector;
	GstElement* m_overlay;
};