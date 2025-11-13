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

 /**
   * SECTION:element-json-convert
   * @title: jsonconvert
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

#include "jsonconvert.h"

GST_DEBUG_CATEGORY_STATIC (gst_json_convert_debug);
#define GST_CAT_DEFAULT gst_json_convert_debug

/* Pad templates: keep it generic for now */
static GstStaticPadTemplate sink_templ = GST_STATIC_PAD_TEMPLATE (
    "sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("ANY"));

static GstStaticPadTemplate src_templ = GST_STATIC_PAD_TEMPLATE (
    "src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("ANY"));

/* Forward decls */
static gboolean gst_json_convert_sink_event (GstPad *pad, GstObject *parent, GstEvent *event);
static GstFlowReturn gst_json_convert_chain (GstPad *pad, GstObject *parent, GstBuffer *buf);

/* Boilerplate type and element registration */
G_DEFINE_TYPE (GstJsonConvert, gst_json_convert, GST_TYPE_ELEMENT);
GST_ELEMENT_REGISTER_DEFINE (json_convert, "jsonconvert", GST_RANK_NONE, GST_TYPE_JSON_CONVERT);

/* Class init */
static void gst_json_convert_class_init (GstJsonConvertClass * klass)
{
    GstElementClass *element_class = GST_ELEMENT_CLASS (klass);

    GST_DEBUG_CATEGORY_INIT (gst_json_convert_debug, "jsonconvert", 0, "JSON convert element");

    gst_element_class_set_static_metadata (
        element_class,
        "JSON Convert",
        "Filter/Converter",
        "Converts incoming data (e.g. JSON/text) for downstream elements",
        "ModularSensorSystem");

    gst_element_class_add_pad_template (element_class, gst_static_pad_template_get (&src_templ));
    gst_element_class_add_pad_template (element_class, gst_static_pad_template_get (&sink_templ));
}

/* Instance init */
static void gst_json_convert_init (GstJsonConvert * self)
{
    self->sinkpad = gst_pad_new_from_static_template (&sink_templ, "sink");
    gst_pad_set_event_function (self->sinkpad, GST_DEBUG_FUNCPTR (gst_json_convert_sink_event));
    gst_pad_set_chain_function (self->sinkpad, GST_DEBUG_FUNCPTR (gst_json_convert_chain));
    GST_PAD_SET_PROXY_CAPS (self->sinkpad);
    gst_element_add_pad (GST_ELEMENT (self), self->sinkpad);

    self->srcpad = gst_pad_new_from_static_template (&src_templ, "src");
    GST_PAD_SET_PROXY_CAPS (self->srcpad);
    gst_element_add_pad (GST_ELEMENT (self), self->srcpad);

    self->silent = FALSE;
}

static gboolean gst_json_convert_sink_event (GstPad * pad, GstObject * parent, GstEvent * event)
{
    gboolean ret;
    GstJsonConvert *self = GST_JSON_CONVERT (parent);

    GST_LOG_OBJECT (self, "Received %s event: %" GST_PTR_FORMAT, GST_EVENT_TYPE_NAME (event), event);

    switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_CAPS: {
        /* For now just forward */
        ret = gst_pad_event_default (pad, parent, event);
        break;
    }
    default:
        ret = gst_pad_event_default (pad, parent, event);
        break;
    }

    return ret;
}

static GstFlowReturn gst_json_convert_chain (GstPad * pad, GstObject * parent, GstBuffer * buf)
{
    GstJsonConvert *self = GST_JSON_CONVERT (parent);
    if (!self->silent) {
        GST_LOG_OBJECT (self, "Passing through buffer");
    }
    return gst_pad_push (self->srcpad, buf);
}
