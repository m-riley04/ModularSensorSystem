#pragma once

#include <gst/gst.h>

/**
 * @brief Represents the size of a drawn element with width, height, and scale.
 */
typedef struct {
	guint width;
	guint height;
	gint scale;
} Size;