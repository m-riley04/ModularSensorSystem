#pragma once

#include "colors.h"
#include "position.h"
#include "glyphs.h"
#include "canvas.h"
#include "text.h"

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

/**
 * @brief Draws a rectangle onto the canvas at the specified position, size, and color.
 * @param canvas The canvas to draw on.
 * @param pos The position to draw the rectangle at.
 * @param size The size of the rectangle.
 * @param color The color of the rectangle.
 */
static void
draw_rect(Canvas* canvas, Position pos, Size size, Color color)
{
    for (guint y = pos.y; y < pos.y + size.height && y < canvas->size.height; y++) {
        guint8* row = canvas->data + y * canvas->size.width * COLOR_SIZE_BYTES;
        for (guint x = pos.x; x < pos.x + size.width && x < canvas->size.width; x++) {
            guint8* px = row + x * COLOR_SIZE_BYTES;
            set_color(px, color, canvas->format);
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

    // Color based on value
	Color color = COLOR_WHITE;
	const gdouble thresh_low = 0.60, thresh_med = 0.75, thresh_high = 0.90;
    if (value < thresh_low) {
        color = COLOR_GREEN;
    }
    else if (value < thresh_med) {
        color = COLOR_YELLOW;
    }
    else if (value < thresh_high) {
        color = COLOR_ORANGE;
    }
    else {
        color = COLOR_RED;
	}

	// Use draw_rect to draw the bar
    Position pos = { x0, y0 };
    Size size = { bar_w, bar_h };
	draw_rect(canvas, pos, size, color);
}