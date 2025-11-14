#pragma once

/**
 * @brief Represents a position with x and y coordinates.
 */
typedef struct {
    gint x;
    gint y;
} Position;

const Position POSITION_DEFAULT = { 0, 0 };

static Position get_centered_position(guint container_width, guint container_height, guint item_width, guint item_height) {
    Position pos;
    pos.x = (gint)((container_width > item_width) ? (container_width - item_width) / 2 : 0);
    pos.y = (gint)((container_height > item_height) ? (container_height - item_height) / 2 : 0);
    return pos;
}