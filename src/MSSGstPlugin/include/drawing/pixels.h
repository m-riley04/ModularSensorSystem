#pragma once

#include "position.h"
#include "canvas.h"

/**
 * @brief Sets the pixel at the given position in the base buffer to the specified color.
 * @param canvas_buf The base buffer/canvas.
 * @param canvas_size The size of the canvas.
 * @param pos The position of the pixel to set.
 * @param color The color to set the pixel to.
 */
static inline void
set_px(Canvas* canvas, Position pos, Color color)
{
    if (pos.x < 0 || pos.y < 0 ||
        pos.x >= (gint)canvas->size.width ||
        pos.y >= (gint)canvas->size.height)
        return;

    size_t idx = ((size_t)pos.y * canvas->size.width + (size_t)pos.x) * COLOR_SIZE_BYTES;
    guint8* px = canvas->data + idx;
    set_color(px, color, canvas->format);
}