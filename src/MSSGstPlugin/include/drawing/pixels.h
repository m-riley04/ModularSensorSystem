#pragma once

#include "position.h"
#include "size.h"

/**
 * @brief Sets the pixel at the given position in the base buffer to the specified color.
 * @param base The base buffer.
 * @param size The size of the buffer.
 * @param pos The position of the pixel to set.
 * @param color The color to set the pixel to.
 * @param format The format string ("BGRx" or "xRGB").
 */
static inline void
set_px(guint8* base, Size size, Position pos, Color color, const char* format)
{
    if (pos.x < 0 || pos.y < 0 || pos.x >= size.width) return; /* height check by caller */
    guint8* px = base + ((size_t)pos.y * (size_t)size.width + (size_t)pos.x) * COLOR_SIZE_BYTES;
    set_color(px, color, format);
}