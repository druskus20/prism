#include <stdlib.h>

#include "handlers.h"
#include "manager.h"
#include "tile.h"
#include "window.h"

#include "../globals.h"

#include "../xcb/connection.h"
#include "../xcb/ewmh.h"
#include "../xcb/pointer.h"
#include "../xcb/window.h"

void (*xcb_events[XCB_NO_OPERATION])(xcb_generic_event_t*) = {
    [XCB_MAP_REQUEST]   = handle_map_request,

    [XCB_BUTTON_PRESS]      = handle_button_down,
    [XCB_BUTTON_RELEASE]    = handle_button_up,
    [XCB_MOTION_NOTIFY]     = handle_pointer,
};

void (*signals[SIGUNUSED])(void) = {
    [SIGINT]    = handle_termination_signal,
    [SIGTERM]   = handle_termination_signal
};

void handle_map_request(xcb_generic_event_t *generic_event) {
    xcb_map_request_event_t *event;
    event = (xcb_map_request_event_t *)generic_event;

    xcb_window_t window_id = event->window;
    xcb_ewmh_get_atoms_reply_t *window_type = NULL;
    xcb_get_property_cookie_t type_cookie;

    type_cookie = xcb_ewmh_get_wm_window_type(ewmh_connection, window_id);
    xcb_get_window_attributes_reply_t *attributes = get_window_attributes(
        window_id);

    if (attributes && attributes->override_redirect)
        goto map;

    xcb_ewmh_get_wm_window_type_reply(ewmh_connection, type_cookie,
        window_type, NULL);

    if (window_type) {
        unsigned int index = 0;
        xcb_atom_t atom;

        while ((atom = window_type->atoms[index++])) {
            // special case: ewmh_connection->_NET_WM_WINDOW_TYPE_SPLASH
            // centered
            if (atom != ewmh_connection->_NET_WM_WINDOW_TYPE_NORMAL) {
                xcb_ewmh_get_atoms_reply_wipe(window_type);
                goto map;
            }
        }

        xcb_ewmh_get_atoms_reply_wipe(window_type);
    }

    focused_window = window_id;

    window_t *window = manage_window(window_id);
    window_id = window->parent;

    push_to_vector(focused_group->children, window);
    tile_windows(0);

map:
    map_window(window_id);
    flush();
}

void handle_button_down(xcb_generic_event_t *generic_event) {
    log_debug("POINTER");
    xcb_grab_button(xcb_connection, 0, xcb_screen->root,
        XCB_EVENT_MASK_BUTTON_RELEASE |
        XCB_EVENT_MASK_BUTTON_MOTION |
        XCB_EVENT_MASK_POINTER_MOTION_HINT,
        XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC,
        XCB_NONE, XCB_NONE,
        XCB_BUTTON_INDEX_1, /* Left mouse button */
        XCB_MOD_MASK_ANY);
    flush();
}

void handle_button_up(xcb_generic_event_t *generic_event) {
    log_debug("POINTER UP");
    xcb_ungrab_button(xcb_connection, XCB_BUTTON_INDEX_ANY, xcb_screen->root, XCB_MOD_MASK_ANY);
}

void handle_pointer(xcb_generic_event_t *generic_event) {
    xcb_query_pointer_reply_t *p = get_pointer_coordinates();
    /*log_debug("MOTION %d %d",
        p->root_x, p->root_y);*/
    //change_window_geometry(focused_window, p->root_x, p->root_y, 300, 300);
    move_group(focused_group, p->root_x, p->root_y);
    flush();
}

/* Process signal handling */
void handle_termination_signal() {
    window_manager_is_active = 0;
}
