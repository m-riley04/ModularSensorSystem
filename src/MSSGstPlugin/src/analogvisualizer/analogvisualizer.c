/*
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
 * Copyright (C) 2020 Niels De Graef <niels.degraef@gmail.com>
 * Copyright (C) 2025 Riley Meyerkorth <rileymeyerkort@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "analogvisualizer/analogvisualizer.h"
#include "analogvisualizer/analogvisualizerhandlers.h"
#include "glyphs.h"
#include <gst/video/video.h>
#include <string.h>

/**
 * SECTION:element-analog-visualizer
 * @title: analogvisualizer
 *
 * TODO: add description
 * 
 * TODO: Add example launch line
 *
 * ## Example launch line
 * |[
 * gst-launch-1.0 -v audiotestsrc ! audioconvert ! visualizer ! videoconvert ! ximagesink
 * ]|
 *
 */

// NOTE: Look at visualizer's implementation: https://github.com/GStreamer/gstreamer/tree/main/subprojects/gst-plugins-good/gst/visualizer

GST_DEBUG_CATEGORY_STATIC(gst_analog_visualizer_debug);
#define GST_CAT_DEFAULT gst_analog_visualizer_debug

#if G_BYTE_ORDER == G_BIG_ENDIAN
#define RGB_ORDER "xRGB"
#else
#define RGB_ORDER "BGRx"
#endif

#define DEFAULT_WIDTH   640
#define DEFAULT_HEIGHT  480
#define DEFAULT_FPS_N   30
#define DEFAULT_FPS_D   1

/* Pad templates: keep it generic for now */
static GstStaticPadTemplate
sink_template = GST_STATIC_PAD_TEMPLATE(
    "sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS("application/x-double"));

static GstStaticPadTemplate
src_template = GST_STATIC_PAD_TEMPLATE("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS("video/x-raw, "
        "format = (string) " RGB_ORDER ", "
        "width = " G_STRINGIFY(DEFAULT_WIDTH) ", "
        "height = " G_STRINGIFY(DEFAULT_HEIGHT) ", "
        "framerate = " GST_VIDEO_FPS_RANGE)
);

/* Forward decls */
static gboolean gst_analog_visualizer_sink_event(GstPad* pad, GstObject* parent, GstEvent* event);
static gboolean gst_analog_visualizer_src_event(GstPad* pad, GstObject* parent, GstEvent* event);
static GstFlowReturn gst_analog_visualizer_chain(GstPad* pad, GstObject* parent, GstBuffer* buf);
static void gst_analog_visualizer_finalize(GObject* object);
static void gst_analog_visualizer_reset(GstAnalogVisualizer* visualizer);
static GstStateChangeReturn gst_analog_visualizer_change_state(GstElement* element, GstStateChange transition);

/* Boilerplate type and element registration */
#define gst_analog_visualizer_parent_class parent_class
G_DEFINE_TYPE(GstAnalogVisualizer, gst_analog_visualizer, GST_TYPE_ELEMENT);
GST_ELEMENT_REGISTER_DEFINE_WITH_CODE(analog_visualizer, "analogvisualizer", GST_RANK_NONE, 
    GST_TYPE_ANALOG_VISUALIZER, 
    GST_DEBUG_CATEGORY_INIT(gst_analog_visualizer_debug, "analogvisualizer", 0, "Analog visualizer element"););

/* Class init */
static void
gst_analog_visualizer_class_init(GstAnalogVisualizerClass* klass)
{
    // Init base GObject class
    GObjectClass* gobject_class = G_OBJECT(klass);
    gobject_class->finalize = gst_analog_visualizer_finalize;

    // Init gst element class
    GstElementClass* gstelement_class = GST_ELEMENT_CLASS(klass);
    gstelement_class->change_state = GST_DEBUG_FUNCPTR(gst_analog_visualizer_change_state);
    
    // Add pads
    gst_element_class_add_pad_template(gstelement_class, gst_static_pad_template_get(&src_template));
    gst_element_class_add_pad_template(gstelement_class, gst_static_pad_template_get(&sink_template));

    gst_element_class_set_static_metadata(
        gstelement_class,
        "Analog Visualizer",
        "Filter/Visualizer",
        "Visualizes analog data",
        "Riley Meyerkorth <rileymeyerkorth@gmail.com>");
}

/* Instance init */
static void
gst_analog_visualizer_init(GstAnalogVisualizer* self)
{
    self->silent = FALSE;

    self->sinkpad = gst_pad_new_from_static_template(&sink_template, "sink");
    gst_pad_set_event_function(self->sinkpad, GST_DEBUG_FUNCPTR(gst_analog_visualizer_sink_event));
    gst_pad_set_chain_function(self->sinkpad, GST_DEBUG_FUNCPTR(gst_analog_visualizer_chain));
    gst_element_add_pad(GST_ELEMENT(self), self->sinkpad);

    self->srcpad = gst_pad_new_from_static_template(&src_template, "src");
    gst_pad_set_event_function(self->srcpad, GST_DEBUG_FUNCPTR(gst_analog_visualizer_src_event));
    gst_element_add_pad(GST_ELEMENT(self), self->srcpad);

    //self->adapter = gst_adapter_new();
    self->next_ts = 0;
    self->bps = sizeof(gint16);

    /* reset the initial video state */
    self->width = DEFAULT_WIDTH;
    self->height = DEFAULT_HEIGHT;
    self->fps_n = DEFAULT_FPS_N;      /* desired frame rate */
    self->fps_d = DEFAULT_FPS_D;
    //self->visstate = NULL;

    self->rate = 1;
}

static void
gst_analog_visualizer_finalize(GObject* object)
{
    GstAnalogVisualizer* visualizer = GST_ANALOG_VISUALIZER(object);

    // TODO/CONSIDER: Implement?

    G_OBJECT_CLASS(parent_class)->finalize(object);
}

static void
gst_analog_visualizer_reset(GstAnalogVisualizer* visualizer)
{
    visualizer->next_ts = 0;

    //gst_adapter_clear(visualizer->adapter);
    gst_segment_init(&visualizer->segment, GST_FORMAT_UNDEFINED);
    visualizer->segment_pending = FALSE;

    GST_OBJECT_LOCK(visualizer);
    visualizer->earliest_time = -1;
    GST_OBJECT_UNLOCK(visualizer);
}

static gboolean
gst_analog_visualizer_sink_setcaps(GstAnalogVisualizer* visualizer, GstCaps* caps)
{
    GstStructure* structure;

    structure = gst_caps_get_structure(caps, 0);

    gst_structure_get_int(structure, "rate", &visualizer->rate);

    GST_DEBUG_OBJECT(visualizer, "sample rate = %d", visualizer->rate);
    return TRUE;
}

static gboolean
gst_analog_visualizer_src_setcaps(GstAnalogVisualizer* visualizer, GstCaps* caps)
{
    GstStructure* structure;
    gboolean res;

    structure = gst_caps_get_structure(caps, 0);

    gst_structure_get_int(structure, "width", &visualizer->width);
    gst_structure_get_int(structure, "height", &visualizer->height);
    gst_structure_get_fraction(structure, "framerate", &visualizer->fps_n,
        &visualizer->fps_d);

    visualizer->outsize = visualizer->width * visualizer->height * 4;
    visualizer->frame_duration = gst_util_uint64_scale_int(GST_SECOND,
        visualizer->fps_d, visualizer->fps_n);
    visualizer->spf =
        gst_util_uint64_scale_int(visualizer->rate, visualizer->fps_d,
            visualizer->fps_n);

    GST_DEBUG_OBJECT(visualizer, "dimension %dx%d, framerate %d/%d, spf %d",
        visualizer->width, visualizer->height, visualizer->fps_n,
        visualizer->fps_d, visualizer->spf);

    res = gst_pad_set_caps(visualizer->srcpad, caps);

    return res;
}

static gboolean
gst_analog_visualizer_sink_event(GstPad* pad, GstObject* parent, GstEvent* event)
{
    gboolean res = FALSE;
    GstAnalogVisualizer* self = GST_ANALOG_VISUALIZER(parent);

    GST_LOG_OBJECT(self, "Received %s event: %" GST_PTR_FORMAT, GST_EVENT_TYPE_NAME(event), event);

    switch (GST_EVENT_TYPE(event)) {
    case GST_EVENT_FLUSH_START:
        res = gst_pad_push_event(self->srcpad, event);
        break;
    case GST_EVENT_FLUSH_STOP:
        gst_analog_visualizer_reset(self);
        res = gst_pad_push_event(self->srcpad, event);
        break;
    case GST_EVENT_SEGMENT:
    {
		res = handle_event_segment(self, event);
        break;
    }
    case GST_EVENT_CAPS:
    {
		res = handle_event_caps(self, event);
        break;
    }
    default:
        res = gst_pad_push_event(self->srcpad, event);
        break;
    }
    gst_event_unref(event); // callee cleans up
    
    return res;
}

static gboolean
gst_analog_visualizer_src_event(GstPad* pad, GstObject* parent, GstEvent* event)
{
    GstAnalogVisualizer* visualizer;
    gboolean res;

    visualizer = GST_ANALOG_VISUALIZER(parent);

    switch (GST_EVENT_TYPE(event)) {
    case GST_EVENT_QOS: {
		res = handle_event_qos(visualizer, event);
        break;
    }
    default:
        res = gst_pad_push_event(visualizer->sinkpad, event);
        break;
    }

    return res;
}

static GstFlowReturn
gst_analog_visualizer_chain(GstPad* pad, GstObject* parent, GstBuffer* inbuf)
{
    GstAnalogVisualizer* self = GST_ANALOG_VISUALIZER(parent);

    /* If we have a pending segment, push it downstream now */
    if (self->segment_pending) {
        GstEvent* seg_evt = gst_event_new_segment(&self->segment);
        gst_pad_push_event(self->srcpad, seg_evt);
        self->segment_pending = FALSE;
    }

    GstMapInfo inmap;
    gdouble value = 0.0;

    /* Expect exactly one gdouble in the buffer */
    if (!gst_buffer_map(inbuf, &inmap, GST_MAP_READ)) {
        gst_buffer_unref(inbuf);
        return GST_FLOW_ERROR;
    }

    if (inmap.size >= sizeof(gdouble)) {
        memcpy(&value, inmap.data, sizeof(gdouble));
    }
    else {
        GST_WARNING_OBJECT(self, "Input buffer too small (%" G_GSIZE_FORMAT
            "), expected at least %zu",
            inmap.size, sizeof(gdouble));
    }

    gst_buffer_unmap(inbuf, &inmap);
    gst_buffer_unref(inbuf);   /* we’re done with the input buffer */

    if (!self->silent) {
        GST_LOG_OBJECT(self, "Received value: %f", value);
    }

    /* Create output video buffer */
    gsize size = (gsize)self->width * self->height * 4;
    GstBuffer* outbuf = gst_buffer_new_and_alloc(size);
    if (!outbuf) {
        return GST_FLOW_ERROR;
    }

    /* Timestamps: use own running clock */
    GST_BUFFER_PTS(outbuf) = self->next_ts;
    GST_BUFFER_DTS(outbuf) = GST_CLOCK_TIME_NONE;
    GST_BUFFER_DURATION(outbuf) =
        gst_util_uint64_scale_int(GST_SECOND,
            self->fps_d,
            self->fps_n);
    self->next_ts += GST_BUFFER_DURATION(outbuf);

    /* Fill frame */
    GstMapInfo outmap;
    if (!gst_buffer_map(outbuf, &outmap, GST_MAP_WRITE)) {
        gst_buffer_unref(outbuf);
        return GST_FLOW_ERROR;
    }

    memset(outmap.data, 0x00, size);  /* black background */

    gchar text[64];
    g_snprintf(text, sizeof(text), "%.6f", value);

    /* Draw bar and overlay the numeric value */
    //draw_bar_value(outmap.data, self->width, self->height, value);
    draw_text_value(outmap.data, self->width, self->height, text);

    gst_buffer_unmap(outbuf, &outmap);

    /* Push video frame downstream */
    return gst_pad_push(self->srcpad, outbuf);
}

static GstStateChangeReturn
gst_analog_visualizer_change_state(GstElement* element, GstStateChange transition)
{
    GstAnalogVisualizer* visualizer = GST_ANALOG_VISUALIZER(element);
    GstStateChangeReturn ret;

    switch (transition) {
    case GST_STATE_CHANGE_NULL_TO_READY:
        break;
    case GST_STATE_CHANGE_READY_TO_PAUSED:
        gst_analog_visualizer_reset(visualizer);
        break;
    default:
        break;
    }

    ret = GST_ELEMENT_CLASS(parent_class)->change_state(element, transition);

    switch (transition) {
    case GST_STATE_CHANGE_PAUSED_TO_READY:
        if (visualizer->pool) {
            gst_buffer_pool_set_active(visualizer->pool, FALSE);
            gst_object_replace((GstObject**)&visualizer->pool, NULL);
        }
        break;
    case GST_STATE_CHANGE_READY_TO_NULL:
        break;
    default:
        break;
    }

    return ret;
}