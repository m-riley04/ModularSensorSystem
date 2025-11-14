#pragma once

#include "position.h"
#include "size.h"

/**
 * @brief Sets the pixel at the given position in the base buffer to the specified color.
 * @param canvas_buf The base buffer/canvas.
 * @param canvas_size The size of the canvas.
 * @param pos The position of the pixel to set.
 * @param color The color to set the pixel to.
 * @param format The format string ("BGRx" or "xRGB").
 */
static inline void
set_px(Canvas canvas, Position pos, Color color, const char* format)
{
    if (pos.x < 0 || pos.y < 0 || pos.x >= canvas.size.width) return; /* height check by caller */
    guint8* px = canvas.data + ((size_t)pos.y * (size_t)canvas.size.width + (size_t)pos.x) * COLOR_SIZE_BYTES;
    set_color(px, color, format);
}