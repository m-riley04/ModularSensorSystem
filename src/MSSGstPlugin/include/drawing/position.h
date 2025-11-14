#pragma once

#include "canvas.h"

/**
 * @brief Represents a position with x and y coordinates.
 * Cannot be negative.
 */
typedef struct {
    gint x;
    gint y;
} Position;

static const Position POSITION_DEFAULT = { 0, 0 };


