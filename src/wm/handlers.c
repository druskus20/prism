#include <stdlib.h>

#include "handlers.h"
#include "manager.h"
#include "tile.h"
#include "window.h"

#include "../ipc/parsing.h"
#include "../util/logging.h"
#include "../xcb/connection.h"
#include "../xcb/ewmh.h"
#include "../xcb/pointer.h"
#include "../xcb/window.h"

void (*xcb_events[XCB_NO_OPERATION])(xcb_generic_event_t*) = {
    [XCB_MAP_REQUEST]    = handle_map_request,

    [XCB_BUTTON_PRESS]   = handle_button_down,
    [XCB_BUTTON_RELEASE] = handle_button_up,
    [XCB_MOTION_NOTIFY]  = handle_pointer,
};

void (*ipc_commands[IPC_CMD_NULL])(ipc_arg_t**) = {
    [IPC_CMD_QUIT] = handle_quit_command,
};

void (*signals[SIGUNUSED])(void) = {
    [SIGINT]  = handle_termination_signal,
    [SIGTERM] = handle_termination_signal
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

    window_t *window = manage_window(window_id);
    window_id = window->parent;

    push_window_to_group(focused_group, window);

    if (focused_group->children->size == 5) // TESTING
        focused_group = initialize_group(); // TESTING

    focused_window = window;

map:
    map_window(window_id);
    flush();
}

void handle_button_down(xcb_generic_event_t *generic_event) {
    xcb_button_press_event_t *event;
    event = (xcb_button_press_event_t*)generic_event;

xcb_warp_pointer(xcb_connection, XCB_NONE, XCB_NONE, 0, 0, 0, 0,
	focused_group->width, focused_group->height);

    log_debug("POINTER");
    xcb_grab_pointer(xcb_connection, XCB_NONE,
        xcb_screen->root,
        XCB_EVENT_MASK_BUTTON_RELEASE |
        XCB_EVENT_MASK_BUTTON_MOTION |
        XCB_EVENT_MASK_POINTER_MOTION_HINT,
        XCB_GRAB_MODE_ASYNC,
        XCB_GRAB_MODE_ASYNC,
        XCB_NONE,
        XCB_NONE,
        XCB_CURRENT_TIME);
    flush();
}

void handle_button_up(xcb_generic_event_t *generic_event) {
    //xcb_ungrab_pointer(xcb_connection, XCB_CURRENT_TIME);
}

void handle_pointer(xcb_generic_event_t *generic_event) {
    xcb_motion_notify_event_t *event;
    event = (xcb_motion_notify_event_t*)generic_event;

    xcb_query_pointer_reply_t *p = get_pointer_coordinates();
    if (event->state & XCB_BUTTON_MASK_1) {
        move_group(focused_group, p->root_x, p->root_y);
    } else if (event->state & XCB_BUTTON_MASK_3) {
        unsigned int height, width;

	    width = p->root_x - focused_group->x;
	    height = p->root_y - focused_group->y;

        resize_group(focused_group, height, width);
    }

    flush();
}

/* IPC command handling */
void handle_quit_command(ipc_arg_t **args) {
    (void)args;

    window_manager_is_active = 0;
}

/* Process signal handling */
void handle_termination_signal() {
    window_manager_is_active = 0;
}
