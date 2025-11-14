#pragma once

#include <gst/gst.h>
#include "colors.h"

/**
 * @brief Represents the size of a drawn element with width, height, and scale.
 */
typedef struct {
	guint width;
	guint height;
} Size;

/**
 * @brief Represents a drawable canvas with pixel data and size.
 */
typedef struct {
	guint8* data; // The data buffer of the canvas
	Size size;
	ColorFormat format;
} Canvas;