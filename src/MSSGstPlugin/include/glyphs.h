#pragma once

#define DEFAULT_GLYPH_SCALE 3 // scaling factor
#define DEFAULT_GLYPH_SPACING (1 * DEFAULT_GLYPH_SCALE) // spacing between glyphs
#define GLYPH_W_BITS 5 // the unscaled width in bits
#define GLYPH_H_BITS 7 // the unscaled height in bits
#define GLYPH_W (GLYPH_W_BITS * DEFAULT_GLYPH_SCALE) // scaled width
#define GLYPH_H (GLYPH_H_BITS * DEFAULT_GLYPH_SCALE) // scaled height
#define GLYPH_ADVANCE (GLYPH_W + DEFAULT_GLYPH_SPACING) // total advance per glyph in bits/pixels
#define NUM_GLYPHS 16 // total number of glyphs including punctuation and spaces
#define COLOR_SIZE_BYTES 4 // size of a color in bytes (BGRx)

#if G_BYTE_ORDER == G_BIG_ENDIAN
#define RGB_ORDER "xRGB"
#define RGBA_INDEX_A 0
#define RGBA_INDEX_R 1
#define RGBA_INDEX_G 2
#define RGBA_INDEX_B 3
#else
#define RGB_ORDER "BGRx"
#define RGBA_INDEX_B 0
#define RGBA_INDEX_G 1
#define RGBA_INDEX_R 2
#define RGBA_INDEX_A 3
#endif


typedef struct { guint8 rows[GLYPH_H]; } Glyph; /* each row uses low 5 bits */

typedef struct {
    gint x;
    gint y;
} GlyphPosition;

typedef struct {
    gint width;
    gint height;
    gint scale;
} GlyphSize;

typedef struct {
    guint8 r;
    guint8 g;
    guint8 b;
    guint8 a;
} GlyphColor;

const GlyphColor GLYPH_COLOR_WHITE = { 0xFF, 0xFF, 0xFF, 0xFF };
const GlyphColor GLYPH_COLOR_BLACK = { 0x00, 0x00, 0x00, 0xFF };

/**
 * @brief Loads a color into a destination buffer in BGRx format.
 * @param dest The destination buffer (must be at least 4 bytes).
 * @param color The color to load.
 */
static void load_from_glyph_color_bgrx(guint8* dest, GlyphColor color)
{
    dest[RGBA_INDEX_B] = color.b; // B
    dest[RGBA_INDEX_G] = color.g; // G
    dest[RGBA_INDEX_R] = color.r; // R
    dest[RGBA_INDEX_A] = color.a; // A
}

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

static inline void
put_px_bgrx(guint8* base, GlyphSize size, GlyphPosition pos, GlyphColor color)
{
    if (pos.x < 0 || pos.y < 0 || pos.x >= size.width) return; /* height check by caller */
    guint8* px = base + ((size_t)pos.y * (size_t)size.width + (size_t)pos.x) * COLOR_SIZE_BYTES;
	load_from_glyph_color_bgrx(px, color);
}

static void
draw_glyph(guint8* data, GlyphSize size, GlyphPosition pos, Glyph glyph, GlyphColor color)
{
    const gint gw = GLYPH_W_BITS, gh = GLYPH_H_BITS;
    for (gint row = 0; row < gh; ++row) {
        if (pos.y + row * size.scale >= size.height) break;
        guint8 rowbits = glyph.rows[row] & 0x1F; /* 5 bits */
        for (gint col = 0; col < gw; ++col) {
            if (rowbits & (1 << (gw - 1 - col))) {
                for (gint sy = 0; sy < size.scale; ++sy) {
                    gint py = pos.y + row * size.scale + sy;
                    if (py < 0 || py >= size.height) continue;
                    for (gint sx = 0; sx < size.scale; ++sx) {
                        gint px = pos.x + col * size.scale + sx;
                        if (px < 0 || px >= size.width) continue;
						GlyphPosition gp = { px, py };
                        put_px_bgrx(data, size, gp, color);
                    }
                }
            }
        }
    }
}

static void
draw_text_value(guint8* data, GlyphSize size, const gchar* text)
{
    /* Render using a small 5x7 bitmap font, scaled up for readability */
    if (!text) return;

    const gint padding = size.scale;    /* spacing between glyphs */

    gint len = (gint)strlen(text);
    if (len <= 0) return;

    gint total_w = len * GLYPH_W + (len - 1) * padding;
    gint x0 = (size.width - total_w) / 2;
    gint y0 = size.height - GLYPH_H - (size.height / 20); /* near bottom */

    for (gint i = 0; i < len; i++) {
        Glyph gph = glyph_get(text[i]);
        gint x_start = x0 + i * (GLYPH_W + padding);
		GlyphPosition pos = { x_start, y0 };
        GlyphColor color = GLYPH_COLOR_WHITE;
        draw_glyph(data, size, pos, gph, color);
    }
}

static void
draw_bar_value(guint8* data, GlyphSize size, gdouble value)
{
    /* clamp |value| into [0,1] */
    if (value < 0) value = -value;
    if (value > 1.0) value = 1.0;

    gint bar_w = size.width / 10;
    gint bar_h = (gint)(value * size.height);
    gint x0 = (size.width - bar_w) / 2;
    gint y0 = size.height - bar_h;  /* bottom-aligned */

    for (gint y = y0; y < size.height; y++) {
        guint8* row = data + y * size.width * COLOR_SIZE_BYTES;
        for (gint x = x0; x < x0 + bar_w; x++) {
            guint8* px = row + x * COLOR_SIZE_BYTES;
			load_from_glyph_color_bgrx(px, GLYPH_COLOR_WHITE);
        }
    }
}