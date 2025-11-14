#pragma once

#include <gst/gst.h>
#include "colors.h"
#include "pixels.h"
#include "size.h"

#define GLYPH_W 5 // the unscaled width in bits
#define GLYPH_H 7 // the unscaled height in bits
#define DEFAULT_GLYPH_SCALE 3 // scaling factor
#define DEFAULT_GLYPH_SPACING (1 * DEFAULT_GLYPH_SCALE) // spacing between glyphs
#define NUM_GLYPHS 16 // total number of glyphs including punctuation and spaces

typedef struct { guint8 rows[GLYPH_H]; } Glyph; /* each row uses low 5 bits */

const Size DEFAULT_GLYPH_SIZE = { GLYPH_W, GLYPH_H };

/**
 * @brief Gets the glyph for the given character.
 * @param c The character to get the glyph for.
 * @return 
 */
static Glyph
glyph_get(gchar c)
{
    switch (c) {
    case '0': return (Glyph) { { 0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E } }; /* 01110 / 10001 / 10011 / 10101 / 11001 / 10001 / 01110 */
    case '1': return (Glyph) { { 0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E } };
    case '2': return (Glyph) { { 0x0E, 0x11, 0x01, 0x06, 0x08, 0x10, 0x1F } };
    case '3': return (Glyph) { { 0x1E, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x1E } };
    case '4': return (Glyph) { { 0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02 } };
    case '5': return (Glyph) { { 0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E } };
    case '6': return (Glyph) { { 0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E } };
    case '7': return (Glyph) { { 0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08 } };
    case '8': return (Glyph) { { 0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E } };
    case '9': return (Glyph) { { 0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C } };
    case '.': return (Glyph) { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06 } }; /* small block bottom */
    case '-': return (Glyph) { { 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00 } };
    case '+': return (Glyph) { { 0x00, 0x04, 0x04, 0x1F, 0x04, 0x04, 0x00 } };
    case 'e': return (Glyph) { { 0x00, 0x0E, 0x10, 0x1E, 0x10, 0x0E, 0x00 } };
    case 'E': return (Glyph) { { 0x1F, 0x10, 0x1E, 0x10, 0x1F, 0x00, 0x00 } };
    case ' ': return (Glyph) { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
    default:  return (Glyph) { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }; /* unknown -> space */
    }
}

/**
 * @brief Draws a single glyph onto the given data buffer at the specified position.
 * @param canvas_data The data buffer of the canvas to draw on.
 * @param canvas_size The size of the canvas being drawn to.
 * @param pos The position to draw the glyph at.
 * @param glyph The glyph to draw.
 * @param color The color to draw the glyph with.
 */
static void
draw_glyph(Canvas canvas, gint scale, Position pos, Glyph glyph, Color color, ColorFormat format)
{
    for (gint row = 0; row < GLYPH_H; ++row) {
        if (pos.y + row * scale >= canvas.size.height) break;
        guint8 rowbits = glyph.rows[row] & 0x1F; /* 5 bits */
        for (gint col = 0; col < GLYPH_W; ++col) {
            if (rowbits & (1 << (GLYPH_W - 1 - col))) {
                for (gint sy = 0; sy < scale; ++sy) {
                    gint py = pos.y + row * scale + sy;
                    if (py < 0 || py >= canvas.size.height) continue;
                    for (gint sx = 0; sx < scale; ++sx) {
                        gint px = pos.x + col * scale + sx;
                        if (px < 0 || px >= canvas.size.width) continue;
                        Position gp = { px, py };
                        set_px(canvas, gp, color, format);
                    }
                }
            }
        }
    }
}