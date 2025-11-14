#pragma once

#include <gst/gst.h>
#include "analogvisualizer.h"

#if G_BYTE_ORDER == G_BIG_ENDIAN
#define RGB_ORDER "xRGB"
#else
#define RGB_ORDER "BGRx"
#endif

/* Forward Declarations */
static gboolean gst_analog_visualizer_sink_setcaps(GstAnalogVisualizer* visualizer, GstCaps* caps);
static gboolean gst_analog_visualizer_src_setcaps(GstAnalogVisualizer* visualizer, GstCaps* caps);

/* Event handlers */
static gboolean handle_event_segment(GstAnalogVisualizer* visualizer, GstEvent* event) {
    /* the newsegment values are used to clip the input samples
         * and to convert the incoming timestamps to running time so
         * we can do QoS */
    gst_event_copy_segment(event, &visualizer->segment);

    /* We forward the event from the chain function after caps are
     * negotiated. Otherwise we would potentially break the event order and
     * send the segment event before the caps event */
    visualizer->segment_pending = TRUE;

    return TRUE;
}

static gboolean handle_event_caps(GstAnalogVisualizer* visualizer, GstEvent* event) {
    GstCaps* in_caps;
    gst_event_parse_caps(event, &in_caps);

    /* Store / inspect upstream caps if you want (currently only reads "rate") */
    gst_analog_visualizer_sink_setcaps(visualizer, in_caps);

    /* Now announce our *output* video format downstream */
    GstCaps* out_caps = gst_caps_new_simple(
        "video/x-raw",
        "format", G_TYPE_STRING, RGB_ORDER, /* "BGRx" or "xRGB" */
        "width", G_TYPE_INT, visualizer->width,
        "height", G_TYPE_INT, visualizer->height,
        "framerate", GST_TYPE_FRACTION, visualizer->fps_n, visualizer->fps_d,
        NULL);

    /* Update internal state and set caps on our srcpad */
    gst_analog_visualizer_src_setcaps(visualizer, out_caps);

    /* Push CAPS event downstream so videoconvert/d3dvideosink can configure */
    gst_pad_push_event(visualizer->srcpad, gst_event_new_caps(out_caps));

    gst_caps_unref(out_caps);
    gst_event_unref(event);
    return TRUE;
}

static gboolean handle_event_qos(GstAnalogVisualizer* visualizer, GstEvent* event) {
    gdouble proportion;
    GstClockTimeDiff diff;
    GstClockTime timestamp;

    gst_event_parse_qos(event, NULL, &proportion, &diff, &timestamp);

    /* save stuff for the _chain() function */
    GST_OBJECT_LOCK(visualizer);
    if (diff >= 0)
        /* we're late, this is a good estimate for next displayable
         * frame (see part-qos.txt) */
        visualizer->earliest_time =
        timestamp + MIN(2 * diff, GST_SECOND) + visualizer->frame_duration;
    else
        visualizer->earliest_time = timestamp + diff;
    GST_OBJECT_UNLOCK(visualizer);

    return gst_pad_push_event(visualizer->sinkpad, event);
}
