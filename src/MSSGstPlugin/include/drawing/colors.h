#pragma once

#include <gst/gst.h>

#define COLOR_SIZE_BYTES 4 // size of a color in bytes (BGRx/xRGB)

/**
 * @brief Represents a colors with red, green, blue, and alpha components.
 * Can represent both xRGB and BGRx formats.
 */
typedef struct {
    guint8 r;
    guint8 g;
    guint8 b;
    guint8 a;
} Color;

/**
 * @brief Represents a color format string.
 * Really, only "BGRx" and "xRGB" are supported.
 * Basically big endian vs little endian.
 */
typedef const char* ColorFormat;

/* Predefined constant colors */
const Color COLOR_WHITE = { 0xFF, 0xFF, 0xFF, 0xFF };
const Color COLOR_BLACK = { 0x00, 0x00, 0x00, 0xFF };
const Color COLOR_RED = { 0xFF, 0x00, 0x00, 0xFF };
const Color COLOR_GREEN = { 0x00, 0xFF, 0x00, 0xFF };
const Color COLOR_BLUE = { 0x00, 0x00, 0xFF, 0xFF };
const Color COLOR_YELLOW = { 0xFF, 0xFF, 0x00, 0xFF };

/**
 * @brief Loads a color into a destination buffer in BGRx format.
 * @param dest The destination buffer (must be at least 4 bytes).
 * @param color The color to load.
 */
static void set_color_bgrx(guint8* dest, Color color)
{
    dest[0] = color.b; // B
    dest[1] = color.g; // G
    dest[2] = color.r; // R
    dest[3] = color.a; // A
}

/**
 * @brief Loads a color into a destination buffer in xRGB format.
 * @param dest The destination buffer (must be at least 4 bytes).
 * @param color The color to load.
 */
static void set_color_xrgb(guint8* dest, Color color)
{
    dest[0] = color.a; // A/X
    dest[1] = color.r; // R
    dest[2] = color.g; // G
    dest[3] = color.b; // B
}

/**
 * @brief Loads a color into a destination buffer based on the specified format.
 * @param dest The destination buffer (must be at least 4 bytes).
 * @param color The color to load.
 * @param format The format string ("BGRx" or "xRGB").
 */
static void set_color(guint8* dest, Color color, const char* format)
{
    if (g_strcmp0(format, "BGRx") == 0) {
        set_color_bgrx(dest, color);
    } else if (g_strcmp0(format, "xRGB") == 0) {
        set_color_xrgb(dest, color);
	}
}