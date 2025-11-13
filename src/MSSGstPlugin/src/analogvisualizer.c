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

#include "analogvisualizer.h"
#include <gst/video/video.h>

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
 * gst-launch-1.0 -v audiotestsrc ! audioconvert ! monoscope ! videoconvert ! ximagesink
 * ]|
 *
 */

// NOTE: Look at monoscope's implementation: https://github.com/GStreamer/gstreamer/tree/main/subprojects/gst-plugins-good/gst/monoscope

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
static GstFlowReturn gst_analog_visualizer_chain(GstPad* pad, GstObject* parent, GstBuffer* buf);
static void draw_text_value(guint8* data, gint width, gint height, const gchar* text);

/* Boilerplate type and element registration */
G_DEFINE_TYPE(GstAnalogVisualizer, gst_analog_visualizer, GST_TYPE_ELEMENT);
GST_ELEMENT_REGISTER_DEFINE(analog_visualizer, "analogvisualizer", GST_RANK_NONE, GST_TYPE_ANALOG_VISUALIZER);

/* Class init */
static void
gst_analog_visualizer_class_init(GstAnalogVisualizerClass* klass)
{
    GstElementClass* element_class = GST_ELEMENT_CLASS(klass);

    GST_DEBUG_CATEGORY_INIT(gst_analog_visualizer_debug, "analogvisualizer", 0, "Analog visualizer element");

    gst_element_class_set_static_metadata(
        element_class,
        "Analog Visualizer",
        "Filter/Visualizer",
        "Visualizes analog data",
        "Riley Meyerkorth <rileymeyerkorth@gmail.com>");

    gst_element_class_add_pad_template(element_class, gst_static_pad_template_get(&src_template));
    gst_element_class_add_pad_template(element_class, gst_static_pad_template_get(&sink_template));
}

/* Instance init */
static void
gst_analog_visualizer_init(GstAnalogVisualizer* self)
{
    self->width = DEFAULT_WIDTH;
    self->height = DEFAULT_HEIGHT;
    self->fps_n = DEFAULT_FPS_N;
    self->fps_d = DEFAULT_FPS_D;
    self->silent = FALSE;
    self->next_ts = 0;

    self->sinkpad = gst_pad_new_from_static_template(&sink_template, "sink");
    gst_pad_set_event_function(self->sinkpad, GST_DEBUG_FUNCPTR(gst_analog_visualizer_sink_event));
    gst_pad_set_chain_function(self->sinkpad, GST_DEBUG_FUNCPTR(gst_analog_visualizer_chain));
    GST_PAD_SET_PROXY_CAPS(self->sinkpad);
    gst_element_add_pad(GST_ELEMENT(self), self->sinkpad);

    self->srcpad = gst_pad_new_from_static_template(&src_template, "src");
    GST_PAD_SET_PROXY_CAPS(self->srcpad);
    gst_element_add_pad(GST_ELEMENT(self), self->srcpad);
}

static gboolean
gst_analog_visualizer_sink_event(GstPad* pad, GstObject* parent, GstEvent* event)
{
    gboolean ret = FALSE;
    GstAnalogVisualizer* self = GST_ANALOG_VISUALIZER(parent);

    GST_LOG_OBJECT(self, "Received %s event: %" GST_PTR_FORMAT, GST_EVENT_TYPE_NAME(event), event);

    switch (GST_EVENT_TYPE(event)) {
    case GST_EVENT_CAPS: {
        /* For now just forward */
        ret = gst_pad_event_default(pad, parent, event);
        break;
    }
    default:
        ret = gst_pad_event_default(pad, parent, event);
        break;
    }

    return ret;
}

static GstFlowReturn
gst_analog_visualizer_chain(GstPad* pad, GstObject* parent, GstBuffer* inbuf)
{
    GstAnalogVisualizer* self = GST_ANALOG_VISUALIZER(parent);
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
    if (self->next_ts == 0) {
        self->next_ts = 0;
    }

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

    draw_text_value(outmap.data, self->width, self->height, text);

    gst_buffer_unmap(outbuf, &outmap);

    /* Push video frame downstream */
    return gst_pad_push(self->srcpad, outbuf);
}

static void
draw_text_value(guint8* data,
    gint width,
    gint height,
    const gchar* text)
{
    /* This is intentionally super dumb: draw white blocks for each char. */
    const gint char_w = 8;
    const gint char_h = 12;
    const gint padding = 4;

    gint len = (gint)strlen(text);
    gint total_w = len * (char_w + padding);

    gint x0 = (width - total_w) / 2;
    gint y0 = (height - char_h) / 2;

    for (gint i = 0; i < len; i++) {
        if (text[i] == ' ')
            continue;

        gint x_start = x0 + i * (char_w + padding);

        for (gint y = 0; y < char_h; y++) {
            gint yy = y0 + y;
            if (yy < 0 || yy >= height)
                continue;

            guint8* row = data + yy * width * 4;

            for (gint x = 0; x < char_w; x++) {
                gint xx = x_start + x;
                if (xx < 0 || xx >= width)
                    continue;

                guint8* px = row + xx * 4;
                px[0] = 0xFF; /* B */
                px[1] = 0xFF; /* G */
                px[2] = 0xFF; /* R */
                px[3] = 0xFF; /* A */
            }
        }
    }
}