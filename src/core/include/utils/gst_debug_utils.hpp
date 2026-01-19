#pragma once

#include <gst/gst.h>
#include <QString>

static inline QString gstCapsToQString(const GstCaps* caps)
{
	if (!caps) return "(null)";
	gchar* s = gst_caps_to_string(const_cast<GstCaps*>(caps));
	QString out = s ? QString::fromUtf8(s) : "(unprintable)";
	g_free(s);
	return out;
}

static inline QString gstPadCapsSummary(GstPad* pad)
{
	if (!pad) return "pad=(null)";

	QString name = QString::fromUtf8(GST_PAD_NAME(pad));
	QString parentName;
	if (GstObject* parent = gst_pad_get_parent(pad)) {
		parentName = QString::fromUtf8(GST_OBJECT_NAME(parent));
		gst_object_unref(parent);
	}
	else {
		parentName = "(no-parent)";
	}

	GstCaps* cur = gst_pad_get_current_caps(pad);
	GstCaps* tmpl = gst_pad_get_pad_template_caps(pad);
	GstCaps* allowed = gst_pad_query_caps(pad, nullptr);

	QString out;
	out += parentName + ":" + name;
	out += " current=" + gstCapsToQString(cur);
	out += " allowed=" + gstCapsToQString(allowed);
	out += " template=" + gstCapsToQString(tmpl);

	if (cur) gst_caps_unref(cur);
	if (tmpl) gst_caps_unref(tmpl);
	if (allowed) gst_caps_unref(allowed);

	return out;
}
