#pragma once

#include <xcb/xcb.h>

#include "group.h"

typedef struct {
    xcb_window_t id;
    xcb_window_t parent;
    float x, y, height, width; /* X and Y are relative to the group */
} window_t;

window_t *manage_window(xcb_window_t);
