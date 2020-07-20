#include <xcb/xcb.h>

#include "connection.h"

#include "../globals.h"

xcb_connection_t *xcb_connection;
xcb_screen_t *xcb_screen;
xcb_visualtype_t *screen_visual;
xcb_colormap_t screen_colormap;
int xcb_file_descriptor;

unsigned short initialize_xcb() {
    if (!(xcb_connection = xcb_connect(NULL, NULL)) ||
        xcb_connection_has_error(xcb_connection)) {
        log_fatal("Unable to create XCB connection. Is the X server running?");
        return 0;
    }

    xcb_screen = xcb_setup_roots_iterator(xcb_get_setup(xcb_connection)).data;

    unsigned int events =
        XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
        XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY   |
        XCB_EVENT_MASK_BUTTON_PRESS          |
        XCB_EVENT_MASK_ENTER_WINDOW          |
        XCB_EVENT_MASK_PROPERTY_CHANGE;

    xcb_void_cookie_t window_attributes_cookie;
    window_attributes_cookie = xcb_change_window_attributes(xcb_connection,
        xcb_screen->root, XCB_CW_EVENT_MASK, &events);

    // Check for --replace flag?
    if (xcb_request_check(xcb_connection, window_attributes_cookie)) {
        log_fatal("%s %s"
            "It appears another window manager is running.",
            "You can try running prism with --replace to kill it.");
        return 0;
    }

    events ^= XCB_EVENT_MASK_ENTER_WINDOW;

    xcb_depth_iterator_t depth_iterator = xcb_screen_allowed_depths_iterator(
        xcb_screen);

    if (depth_iterator.data) {
        xcb_visualtype_iterator_t visual_iterator;

        while (depth_iterator.rem) {
            if (depth_iterator.data->depth == 32) {
                visual_iterator = xcb_depth_visuals_iterator(
                    depth_iterator.data);

                screen_visual = visual_iterator.data;
            }

            xcb_depth_next(&depth_iterator);
        }
    }

    if (!screen_visual) {
        log_fatal("Unable to obtain a screen visual with depth of 32.");
        return 0;
    }

    screen_colormap = xcb_generate_id(xcb_connection);
    xcb_create_colormap(xcb_connection, XCB_COLORMAP_ALLOC_NONE,
        screen_colormap, xcb_screen->root, screen_visual->visual_id);

    xcb_ungrab_key(xcb_connection, XCB_GRAB_ANY, xcb_screen->root,
        XCB_MOD_MASK_ANY);

    xcb_file_descriptor = xcb_get_file_descriptor(xcb_connection);

    return 1;
}

void finalize_xcb() {
    if (!xcb_connection)
        return;

    xcb_free_colormap(xcb_connection, screen_colormap);
    xcb_disconnect(xcb_connection);
}

void flush() {
    xcb_flush(xcb_connection);
}
