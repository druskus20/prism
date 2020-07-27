#include "group.h"
#include "manager.h"
#include "tile.h"
#include "window.h"

#include "../prism.h"

#include "../xcb/window.h"

// TODO: master/stack tiling?

void split_current_view_port(split_mode_t splitting_mode, group_t *group,
    window_t *originating_window, window_t *introducing_window) {

    float height = originating_window->height / 2.0;
    float width = originating_window->width / 2.0;

    switch (splitting_mode) {
        case NORTHERN_SPLIT:
            introducing_window->x = originating_window->x;
            introducing_window->y = originating_window->y;
            originating_window->y += height;

            originating_window->height = introducing_window->height = height;
            introducing_window->width  = originating_window->width;
            break;
        case SOUTHERN_SPLIT:
            introducing_window->x = originating_window->x;
            introducing_window->y = originating_window->y + height;

            originating_window->height = introducing_window->height = height;
            introducing_window->width  = originating_window->width;
            break;
        case EASTERN_SPLIT:
            introducing_window->x = originating_window->x;
            originating_window->x += width;
            introducing_window->y = originating_window->y;

            introducing_window->height = originating_window->height;
            introducing_window->width  = originating_window->width = width;
            break;
        case WESTERN_SPLIT:
            introducing_window->x = originating_window->x + width;
            introducing_window->y = originating_window->y;

            introducing_window->height = originating_window->height;
            introducing_window->width  = originating_window->width = width;
            break;
    }

    change_managed_window_geometry(originating_window,
        originating_window->x + group->x, originating_window->y + group->y,
        originating_window->height,       originating_window->width);

    change_managed_window_geometry(introducing_window,
        introducing_window->x + group->x, introducing_window->y + group->y,
        introducing_window->height,       introducing_window->width);
}
