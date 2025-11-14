#pragma once

#define GLYPH_W  (5 * 3)   // scaled width
#define GLYPH_H  (7 * 3)   // scaled height
#define NUM_GLYPHS 16      // digits + punctuation, etc.

/* =====================
 * Tiny 5x7 bitmap font
 * ===================== */

typedef struct { guint8 rows[7]; } MiniGlyph; /* each row uses low 5 bits */

static MiniGlyph glyph_get(gchar c)
{
    switch (c) {
    case '0': return (MiniGlyph) { { 0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E } }; /* 01110 / 10001 / 10011 / 10101 / 11001 / 10001 / 01110 */
    case '1': return (MiniGlyph) { { 0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E } };
    case '2': return (MiniGlyph) { { 0x0E, 0x11, 0x01, 0x06, 0x08, 0x10, 0x1F } };
    case '3': return (MiniGlyph) { { 0x1E, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x1E } };
    case '4': return (MiniGlyph) { { 0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02 } };
    case '5': return (MiniGlyph) { { 0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E } };
    case '6': return (MiniGlyph) { { 0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E } };
    case '7': return (MiniGlyph) { { 0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08 } };
    case '8': return (MiniGlyph) { { 0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E } };
    case '9': return (MiniGlyph) { { 0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C } };
    case '.': return (MiniGlyph) { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06 } }; /* small block bottom */
    case '-': return (MiniGlyph) { { 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00 } };
    case '+': return (MiniGlyph) { { 0x00, 0x04, 0x04, 0x1F, 0x04, 0x04, 0x00 } };
    case 'e': return (MiniGlyph) { { 0x00, 0x0E, 0x10, 0x1E, 0x10, 0x0E, 0x00 } };
    case 'E': return (MiniGlyph) { { 0x1F, 0x10, 0x1E, 0x10, 0x1F, 0x00, 0x00 } };
    case ' ': return (MiniGlyph) { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
    default:  return (MiniGlyph) { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }; /* unknown -> space */
    }
}

static inline void put_px_bgrx(guint8* base, gint width, gint x, gint y,
    guint8 r, guint8 g, guint8 b)
{
    if (x < 0 || y < 0 || x >= width) return; /* height check by caller */
    guint8* px = base + ((size_t)y * (size_t)width + (size_t)x) * 4;
    px[0] = b; px[1] = g; px[2] = r; px[3] = 0xFF; /* BGRx */
}

static void draw_glyph(guint8* data, gint width, gint height,
    gint x, gint y, MiniGlyph g, gint scale,
    guint8 r, guint8 gg, guint8 b)
{
    const gint gw = 5, gh = 7;
    for (gint row = 0; row < gh; ++row) {
        if (y + row * scale >= height) break;
        guint8 rowbits = g.rows[row] & 0x1F; /* 5 bits */
        for (gint col = 0; col < gw; ++col) {
            if (rowbits & (1 << (gw - 1 - col))) {
                for (gint sy = 0; sy < scale; ++sy) {
                    gint py = y + row * scale + sy;
                    if (py < 0 || py >= height) continue;
                    for (gint sx = 0; sx < scale; ++sx) {
                        gint px = x + col * scale + sx;
                        if (px < 0 || px >= width) continue;
                        put_px_bgrx(data, width, px, py, r, gg, b);
                    }
                }
            }
        }
    }
}

static void
draw_text_value(guint8* data,
    gint width,
    gint height,
    const gchar* text)
{
    /* Render using a small 5x7 bitmap font, scaled up for readability */
    if (!text) return;

    const gint scale = 3;          /* scale glyphs */
    const gint gw = 5 * scale;     /* glyph width in pixels */
    const gint gh = 7 * scale;     /* glyph height in pixels */
    const gint padding = scale;    /* spacing between glyphs */

    gint len = (gint)strlen(text);
    if (len <= 0)
        return;

    gint total_w = len * gw + (len - 1) * padding;
    gint x0 = (width - total_w) / 2;
    gint y0 = height - gh - (height / 20); /* near bottom */

    guint8 r = 0xFF, g = 0xFF, b = 0xFF; /* white */

    for (gint i = 0; i < len; i++) {
        MiniGlyph gph = glyph_get(text[i]);
        gint x_start = x0 + i * (gw + padding);
        draw_glyph(data, width, height, x_start, y0, gph, scale, r, g, b);
    }
}

static void
draw_bar_value(guint8* data,
    gint width,
    gint height,
    gdouble value)
{
    /* clamp |value| into [0,1] */
    if (value < 0) value = -value;
    if (value > 1.0) value = 1.0;

    gint bar_w = width / 10;
    gint bar_h = (gint)(value * height);
    gint x0 = (width - bar_w) / 2;
    gint y0 = height - bar_h;  /* bottom-aligned */

    for (gint y = y0; y < height; y++) {
        guint8* row = data + y * width * 4;
        for (gint x = x0; x < x0 + bar_w; x++) {
            guint8* px = row + x * 4;
            px[0] = 0xFF; // B
            px[1] = 0xFF; // G
            px[2] = 0xFF; // R
            px[3] = 0xFF; // A
        }
    }
}