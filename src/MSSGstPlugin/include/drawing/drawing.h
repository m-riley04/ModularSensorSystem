#pragma once

#include "colors.h"
#include "position.h"
#include "glyphs.h"
#include "canvas.h"

/**
 * @brief Clears the entire canvas to black.
 */
static void
clear_canvas(Canvas* canvas)
{
	guint8* px = canvas->data;
    for (guint y = 0; y < canvas->size.height; y++) {
        guint8* row = canvas->data + y * canvas->size.width * COLOR_SIZE_BYTES;
        for (guint x = 0; x < canvas->size.width; x++) {
            guint8* px = row + x * COLOR_SIZE_BYTES;
            set_color(px, COLOR_BLACK, canvas->format);
        }
    }
}

static void
draw_bar_value(Canvas* canvas, gdouble value)
{
    /* clamp |value| into [0,1] */
    if (value < 0) value = -value;
    if (value > 1.0) value = 1.0;

    guint bar_w = canvas->size.width / 10;
    guint bar_h = (guint)(value * canvas->size.height);
    guint x0 = (canvas->size.width - bar_w) / 2;
    guint y0 = canvas->size.height - bar_h;  /* bottom-aligned */

    for (guint y = y0; y < canvas->size.height; y++) {
        guint8* row = canvas->data + y * canvas->size.width * COLOR_SIZE_BYTES;
        for (guint x = x0; x < x0 + bar_w; x++) {
            guint8* px = row + x * COLOR_SIZE_BYTES;
            set_color(px, COLOR_WHITE, canvas->format);
        }
    }
}

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

	g_print("Drawing text: '%s' at (%d, %d)\n", text, x0, y0);
	g_print("Canvas size: %ux%u\n", canvas->size.width, canvas->size.height);
	g_print("Color format: %s\n", canvas->format);

    for (gint i = 0; i < len; i++) {
        Glyph gph = glyph_get(text[i]);
        gint x_start = x0 + i * (scaled_glyph_w + padding);
        Position pos = { x_start, y0 };
        draw_glyph(canvas, scale, pos, gph, COLOR_RED);
    }
}