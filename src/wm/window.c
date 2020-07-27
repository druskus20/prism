#include <stdlib.h>

#include "manager.h"
#include "group.h"
#include "window.h"

#include "../prism.h"

#include "../xcb/connection.h"
#include "../xcb/window.h"

window_t *manage_window(xcb_window_t window_id) {
    window_t *window = calloc(1, sizeof(window_t));

    window->id = window_id;
    window->parent = xcb_generate_id(xcb_connection);

    window->x      = window->y     = 0;
    window->height = window->width = 1; // We don't know yet

    /* Create the parent window */

    unsigned int masked_values =
        XCB_CW_BACK_PIXEL        |
        XCB_CW_BORDER_PIXEL      |
        XCB_CW_OVERRIDE_REDIRECT |
        XCB_CW_COLORMAP;

    unsigned int values[] = {
        0xffffff << (focused_group->children->size * 0xF), 0, 1, screen_colormap
    };

    xcb_create_window(xcb_connection, 32, window->parent, xcb_screen->root,
        0, 0, 300, 300,
        0, /* Border size */
        XCB_WINDOW_CLASS_INPUT_OUTPUT, screen_visual->visual_id,
        masked_values, values);

    values[0] = XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY;
    configure_window(window->parent, XCB_CW_EVENT_MASK, values);

    xcb_reparent_window(xcb_connection, window->id, window->parent, 0, 0);
    map_window(window->id);

    push_to_vector(managed_windows, window);

    return window;
}
