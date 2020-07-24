#pragma once

typedef enum {
    SPLIT_UP,
    SPLIT_DOWN,
    SPLIT_LEFT,
    SPLIT_RIGHT
} split_mode_t;

void tile_windows(split_mode_t);
