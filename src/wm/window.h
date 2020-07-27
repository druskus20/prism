#pragma once

#include <xcb/xcb.h>

#include "group.h"

typedef struct {
    xcb_window_t id;
    xcb_window_t parent;
    float x, y, height, width; /* X and Y are relative to the group */
} window_t;

window_t *manage_window(xcb_window_t);
void change_managed_window_coordinates(window_t*, float, float);
void change_managed_window_dimensions(window_t*, float, float);
void change_managed_window_geometry(window_t*, float, float, float, float);
