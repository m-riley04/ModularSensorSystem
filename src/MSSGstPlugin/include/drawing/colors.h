#pragma once

#include <gst/gst.h>

#define COLOR_SIZE_BYTES 4 // size of a color in bytes (BGRx/xRGB)

/**
 * @brief Represents a colors with red, green, blue, and alpha components.
 * Can represent both xRGB and BGRx formats.
 * TODO/CONSIDER: instead of 'a', use 'x'?
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
typedef enum {
    COLOR_FORMAT_BGRX,
    COLOR_FORMAT_XRGB
} ColorFormat;

/* Predefined constant colors */
// TODO: maybe make these clearer that they're RGBA?
static const Color COLOR_WHITE = { 0xFF, 0xFF, 0xFF, 0xFF };
static const Color COLOR_BLACK = { 0x00, 0x00, 0x00, 0xFF };
static const Color COLOR_RED = { 0xFF, 0x00, 0x00, 0xFF };
static const Color COLOR_ORANGE = { 0xFF, 0x80, 0x00, 0xFF };
static const Color COLOR_YELLOW = { 0xFF, 0xFF, 0x00, 0xFF };
static const Color COLOR_GREEN = { 0x00, 0xFF, 0x00, 0xFF };
static const Color COLOR_BLUE = { 0x00, 0x00, 0xFF, 0xFF };

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
static void set_color(guint8* dest, Color color, ColorFormat format)
{
    switch (format) {
		case COLOR_FORMAT_XRGB:
			return set_color_xrgb(dest, color);
        case COLOR_FORMAT_BGRX:
            return set_color_bgrx(dest, color);
        default:
            g_warning("Unsupported color format: %d", format);
			return set_color_xrgb(dest, color); // default to BGRx - lil endian
    }
}

static ColorFormat parse_color_format(const char* formatStr)
{
    if (g_strcmp0(formatStr, "BGRx") == 0) {
        return COLOR_FORMAT_BGRX;
    }
    else if (g_strcmp0(formatStr, "xRGB") == 0) {
        return COLOR_FORMAT_XRGB;
    }
    else {
        g_warning("Unknown color format string: %s. Defaulting to BGRx.", formatStr);
        return COLOR_FORMAT_BGRX;
    }
}