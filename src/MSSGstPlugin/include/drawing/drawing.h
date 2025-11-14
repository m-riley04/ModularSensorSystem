#pragma once

#include "colors.h"
#include "position.h"
#include "glyphs.h"
#include "size.h"

static void
draw_bar_value(guint8* data, gdouble value, Size size, ColorFormat format)
{
    /* clamp |value| into [0,1] */
    if (value < 0) value = -value;
    if (value > 1.0) value = 1.0;

    gint bar_w = size.width / 10;
    gint bar_h = (gint)(value * size.height);
    gint x0 = (size.width - bar_w) / 2;
    gint y0 = size.height - bar_h;  /* bottom-aligned */

    for (gint y = y0; y < size.height; y++) {
        guint8* row = data + y * size.width * COLOR_SIZE_BYTES;
        for (gint x = x0; x < x0 + bar_w; x++) {
            guint8* px = row + x * COLOR_SIZE_BYTES;
            set_color(px, COLOR_WHITE, format);
        }
    }
}

static void
draw_text_value(guint8* data, const gchar* text, Size size, ColorFormat format)
{
    /* Render using a small bitmap font, scaled up for readability */
    if (!text) return;

    const gint padding = size.scale;    /* spacing between glyphs */

    gint len = (gint)strlen(text);
    if (len <= 0) return;

    gint total_w = len * GLYPH_W + (len - 1) * padding;
    gint x0 = (size.width - total_w) / 2;
    gint y0 = size.height - GLYPH_H - (size.height / 20); /* near bottom */

    for (gint i = 0; i < len; i++) {
        Glyph gph = glyph_get(text[i]);
        gint x_start = x0 + i * (GLYPH_W + padding);
        Position pos = { x_start, y0 };
        draw_glyph(data, size, pos, gph, COLOR_WHITE, format);
    }
}