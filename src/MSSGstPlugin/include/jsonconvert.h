#pragma once

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>

typedef struct _GstJsonConvert {
	GstElement element;

	GstPad* sinkpad, * srcpad;

	gboolean silent;
} GstJsonConvert;

typedef struct _GstJsonConvertClass {
	GstElementClass parent_class;
} GstJsonConvertClass;

/* Standard macros for defining types for this element.  */
#define GST_TYPE_JSON_CONVERT (gst_json_convert_get_type())
#define GST_JSON_CONVERT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_JSON_CONVERT,GstJsonConvert))
#define GST_JSON_CONVERT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_JSON_CONVERT,GstMyFilterClass))
#define GST_IS_JSON_CONVERT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_JSON_CONVERT))
#define GST_IS_JSON_CONVERT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_JSON_CONVERT))

/* Standard function returning type information. */
GType gst_json_convert_get_type(void);

GST_ELEMENT_REGISTER_DECLARE(json_convert)

static void
gst_json_convert_class_init(GstJsonConvertClass* klass);

