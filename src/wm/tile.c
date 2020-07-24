#include "tile.h"
#include "manager.h"
#include "window.h"

#include "../globals.h"

#include "../xcb/connection.h" // REMOVE THESE
#include "../xcb/window.h"

void tile_windows(split_mode_t splitting_mode) {
    // GET workable area
    /*unsigned int height = xcb_screen->height_in_pixels;
    unsigned int width = xcb_screen->width_in_pixels;*/

    unsigned int height = focused_group->height;
    unsigned int width = focused_group->width;

    // Simple master stack for testing

    unsigned int w = (unsigned int)(width / 2.0);

    window_t *window = NULL;

    window = vector_iterator(focused_group->children);
    if (window) {
        change_window_geometry(window->parent,
            focused_group->x, focused_group->y,
            height, w
        );

        window->x = focused_group->x;
        window->y = focused_group->y;
        window->height= height;
        window->width = w;
    }

    unsigned int i = 0;
    unsigned int h = (unsigned int)(height / (float)focused_group->children->size - 1);

    while ((window = vector_iterator(focused_group->children))) {
        change_window_geometry(window->parent,
            w, (h * i++),
            h, w
        );
        window->x = w; window->y = (h * i);
        window->height = h; window->width = width;
    }
}
