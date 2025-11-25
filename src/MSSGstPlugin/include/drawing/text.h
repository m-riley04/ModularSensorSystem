#pragma once

#include "canvas.h"
#include "glyphs.h"

static void
draw_text_value(Canvas* canvas, const gchar* text, gint scale)
{
    /* Render using a small bitmap font, scaled up for readability */
    if (!text) return;

    const gint padding = scale;    /* spacing between glyphs */

    gint len = (gint)strlen(text);
    if (len <= 0) return;

    gint scaled_glyph_w = GLYPH_W * scale;
    gint scaled_glyph_h = GLYPH_H * scale;

    gint total_w = len * scaled_glyph_w + (len - 1) * padding;
    gint x0 = (canvas->size.width - total_w) / 2;
    gint y0 = canvas->size.height - scaled_glyph_h - (canvas->size.height / 20); /* near bottom */

    for (gint i = 0; i < len; i++) {
        Glyph gph = glyph_get(text[i]);
        gint x_start = x0 + i * (scaled_glyph_w + padding);
        Position pos = { x_start, y0 };
        draw_glyph(canvas, scale, pos, gph, COLOR_WHITE);
    }
}