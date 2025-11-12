#include "jsonconvert.h"

G_DEFINE_TYPE(GstJsonConvert, gst_json_convert, GST_TYPE_ELEMENT);
GST_ELEMENT_REGISTER_DEFINE(json_convert, "jsonconvert", GST_RANK_NONE, GST_TYPE_JSON_CONVERT);

void gst_json_convert_class_init(GstJsonConvertClass* klass)
{
    GstElementClass* element_class = GST_ELEMENT_CLASS(klass);

    // TODO: set element details
    gst_element_class_set_static_metadata(element_class,
        "An example plugin",
        "Example/FirstExample",
        "Shows the basic structure of a plugin",
        "your name <your.name@your.isp>");

    // TOOD: init element class
}

void gst_json_convert_init(GstJsonConvert* klass)
{
    GstElement* element = GST_ELEMENT(klass);

    // TODO: init element
}
