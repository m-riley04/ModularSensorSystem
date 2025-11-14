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

#include "analogtestsrc/analogtestsrc.h"
#include <string.h>

/**
 * SECTION:element-analog-test-src
 * @title: analogtestsrc
 *
 * TODO: add description
 *
 * TODO: Add example launch line
 *
 * ## Example launch line
 * |[
 * gst-launch-1.0 -v analogtestsrc ! analogvisualizer ! videoconvert ! ximagesink
 * ]|
 *
 */

GST_DEBUG_CATEGORY_STATIC(gst_analog_test_src_debug);
#define GST_CAT_DEFAULT gst_analog_test_src_debug

#define GST_ANALOG_TEST_SRC_BUFFER_SIZE 8 // this is the size of the gdouble data type in bytes. TODO: check if this is fine?

static GstStaticPadTemplate
src_template = GST_STATIC_PAD_TEMPLATE(
    "src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS("application/x-double"));


/* Forward decls */
static gboolean gst_analog_test_src_src_event(GstPad* pad, GstObject* parent, GstEvent* event);
static GstFlowReturn gst_analog_test_src_chain(GstPad* pad, GstObject* parent, GstBuffer* buf);
static void gst_analog_test_src_finalize(GObject* object);
static void gst_analog_test_src_reset(GstAnalogTestSrc* visualizer);
static GstStateChangeReturn gst_analog_test_src_change_state(GstElement* element, GstStateChange transition);
static GstFlowReturn handle_output_buf(GstAnalogTestSrc* self, GstBuffer* outbuf, gdouble value);

/* Boilerplate type and element registration */
#define gst_analog_test_src_parent_class parent_class
G_DEFINE_TYPE(GstAnalogTestSrc, gst_analog_test_src, GST_TYPE_ELEMENT);
GST_ELEMENT_REGISTER_DEFINE_WITH_CODE(analog_test_src, "analogtestsrc", GST_RANK_NONE,
    GST_TYPE_ANALOG_TEST_SRC,
    GST_DEBUG_CATEGORY_INIT(gst_analog_test_src_debug, "analogtestsrc", 0, "Analog visualizer element"););

/* Class init */
static void
gst_analog_test_src_class_init(GstAnalogTestSrcClass* klass)
{
    // Init base GObject class
    GObjectClass* gobject_class = G_OBJECT(klass);
    gobject_class->finalize = gst_analog_test_src_finalize;

    // Init gst element class
    GstElementClass* gstelement_class = GST_ELEMENT_CLASS(klass);
    gstelement_class->change_state = GST_DEBUG_FUNCPTR(gst_analog_test_src_change_state);

    // Add pads
    gst_element_class_add_pad_template(gstelement_class, gst_static_pad_template_get(&src_template));

    gst_element_class_set_static_metadata(
        gstelement_class,
        "Analog Test Source",
        "Source",
        "Emits random analog data for testing purposes",
        "Riley Meyerkorth <rileymeyerkorth@gmail.com>");
}

/* Instance init */
static void
gst_analog_test_src_init(GstAnalogTestSrc* self)
{
    self->silent = FALSE;

    self->srcpad = gst_pad_new_from_static_template(&src_template, "src");
    gst_pad_set_event_function(self->srcpad, GST_DEBUG_FUNCPTR(gst_analog_test_src_src_event));
    gst_element_add_pad(GST_ELEMENT(self), self->srcpad);

    // TODO: Implement more?
}

static void
gst_analog_test_src_finalize(GObject* object)
{
    GstAnalogTestSrc* self = GST_ANALOG_TEST_SRC(object);

    // TODO/CONSIDER: Implement?

    G_OBJECT_CLASS(parent_class)->finalize(object);
}

static void
gst_analog_test_src_reset(GstAnalogTestSrc* self)
{
    // TODO: implement?

    GST_OBJECT_LOCK(self);
    self->earliest_time = -1;
    GST_OBJECT_UNLOCK(self);
}

static gboolean
gst_analog_test_src_src_setcaps(GstAnalogTestSrc* self, GstCaps* caps)
{
    GstStructure* structure;
    gboolean res;

    structure = gst_caps_get_structure(caps, 0);

    // TODO: parse caps if needed

    res = gst_pad_set_caps(self->srcpad, caps);

    return res;
}

static gboolean
gst_analog_test_src_src_event(GstPad* pad, GstObject* parent, GstEvent* event)
{
    // TODO: is this the right event logic? It uses sinkpad...

    GstAnalogTestSrc* self;
    gboolean res;

    self = GST_ANALOG_TEST_SRC(parent);

    switch (GST_EVENT_TYPE(event)) {
    case GST_EVENT_QOS: {
        res = handle_event_qos(self, event);
        break;
    }
    default:
        //res = gst_pad_push_event(self->sinkpad, event);
        break;
    }

    return res;
}

static GstFlowReturn
gst_analog_test_src_chain(GstPad* pad, GstObject* parent, GstBuffer* inbuf)
{
    GstAnalogTestSrc* self = GST_ANALOG_TEST_SRC(parent);

    // Generate random value/data
	gdouble value = g_random_double_range(0.0, 1.0);

    // Log
    if (!self->silent) {
        GST_LOG_OBJECT(self, "Generated random value: %f", value);
    }

    /* Create output analog data buffer */
    GstBuffer* outbuf = gst_buffer_new_and_alloc(GST_ANALOG_TEST_SRC_BUFFER_SIZE);
    if (!outbuf) {
        return GST_FLOW_ERROR;
    }

    return handle_output_buf(self, outbuf, value);
}

static GstStateChangeReturn
gst_analog_test_src_change_state(GstElement* element, GstStateChange transition)
{
    GstAnalogTestSrc* self = GST_ANALOG_TEST_SRC(element);
    GstStateChangeReturn ret;

    switch (transition) {
    case GST_STATE_CHANGE_NULL_TO_READY:
        break;
    case GST_STATE_CHANGE_READY_TO_PAUSED:
        gst_analog_test_src_reset(self);
        break;
    default:
        break;
    }

    ret = GST_ELEMENT_CLASS(parent_class)->change_state(element, transition);

    switch (transition) {
    case GST_STATE_CHANGE_PAUSED_TO_READY:
        if (self->pool) {
            gst_buffer_pool_set_active(self->pool, FALSE);
            gst_object_replace((GstObject**)&self->pool, NULL);
        }
        break;
    case GST_STATE_CHANGE_READY_TO_NULL:
        break;
    default:
        break;
    }

    return ret;
}

static GstFlowReturn handle_output_buf(GstAnalogTestSrc* self, GstBuffer* outbuf, gdouble value) {
    GstMapInfo outmap;
    /* Fill output buffer with the generated analog value */
    if (!gst_buffer_map(outbuf, &outmap, GST_MAP_WRITE)) {
        gst_buffer_unref(outbuf);
        return GST_FLOW_ERROR;
    }
    // Copy the gdouble value into the buffer
    memcpy(outmap.data, &value, sizeof(gdouble));
    gst_buffer_unmap(outbuf, &outmap);
    // Push buffer downstream
    GstFlowReturn ret = gst_pad_push(self->srcpad, outbuf);
    return ret;
}

static gboolean handle_event_qos(GstAnalogTestSrc* self, GstEvent* event) {
    //gdouble proportion;
    //GstClockTimeDiff diff;
    //GstClockTime timestamp;

    //gst_event_parse_qos(event, NULL, &proportion, &diff, &timestamp);

    ///* save stuff for the _chain() function */
    //GST_OBJECT_LOCK(self);
    //if (diff >= 0)
    //    /* we're late, this is a good estimate for next displayable
    //     * frame (see part-qos.txt) */
    //    self->earliest_time =
    //    timestamp + MIN(2 * diff, GST_SECOND) + self->frame_duration;
    //else
    //    self->earliest_time = timestamp + diff;
    //GST_OBJECT_UNLOCK(self);

    //return gst_pad_push_event(self->sinkpad, event);
}