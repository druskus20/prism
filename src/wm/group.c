#include <stdlib.h>

#include "group.h"
#include "manager.h"
#include "window.h"

#include "../globals.h"

#include "../xcb/pointer.h"
#include "../xcb/window.h"

group_t *initialize_group() {
    group_t *group = (group_t*)calloc(1, sizeof(group_t));

    group->x      = group->y     = 0;
    group->height = group->width = 500; // 500x500 for testing
    group->children = construct_vector();

    push_to_vector(groups, group);

    return group;
}

void move_group(group_t *group, unsigned int x, unsigned int y) {
    int offset_x, offset_y = 0;

    group->x = x;
    group->y = y;

    window_t *window = NULL;
    while ((window = vector_iterator(group->children))) {
        offset_x = group->x - window->x;
        offset_y = group->y - window->y;

        change_window_geometry(window->parent,
            offset_x, offset_y,
            window->height, window->width);
    }
}
