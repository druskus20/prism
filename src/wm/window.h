#pragma once

#include <xcb/xcb.h>

typedef struct {
    xcb_window_t id;
    xcb_window_t parent;
    unsigned int x, y, height, width;
} window_t;

window_t *manage_window(xcb_window_t);
