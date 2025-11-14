#pragma once

/**
 * @brief Represents a position with x and y coordinates.
 */
typedef struct {
    gint x;
    gint y;
} Position;

const Position POSITION_DEFAULT = { 0, 0 };
