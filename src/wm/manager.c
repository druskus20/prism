#include <unistd.h>

#include <xcb/xcb.h>

#include "group.h"
#include "handlers.h"
#include "manager.h"

#include "../util/logging.h"

#include "../xcb/connection.h"
#include "../xcb/ewmh.h"
#include "../xcb/pointer.h"

unsigned char window_manager_is_active = 1;

vector_t *managed_windows = NULL;
vector_t *groups = NULL;

window_t *focused_window = NULL; // REMOVE
group_t *focused_group = NULL;

unsigned int initialize_prism() {
    if (!initialize_xcb() || !initialize_ewmh()) {
        log_fatal("Unable to initialize window manager.");
        return EXIT_FAILURE;
    }

    managed_windows = construct_vector();
    groups = construct_vector();

    focused_group = initialize_group(); // REMOVE

    unsigned int index = 0;
    for (; index < SIGUNUSED; index++) {
        if (signals[index])
            signal(index, handle_signals);
    }

    /* Attach to pointer, remove at some point */
    initialize_pointer();
    xcb_grab_button(xcb_connection, 0, xcb_screen->root, XCB_EVENT_MASK_BUTTON_PRESS |
        XCB_EVENT_MASK_BUTTON_RELEASE, XCB_GRAB_MODE_ASYNC,
        XCB_GRAB_MODE_ASYNC, xcb_screen->root, XCB_NONE, XCB_BUTTON_INDEX_1, XCB_MOD_MASK_ANY);

    xcb_grab_button(xcb_connection, 0, xcb_screen->root, XCB_EVENT_MASK_BUTTON_PRESS |
            XCB_EVENT_MASK_BUTTON_RELEASE, XCB_GRAB_MODE_ASYNC,
            XCB_GRAB_MODE_ASYNC, xcb_screen->root, XCB_NONE, XCB_BUTTON_INDEX_3, XCB_MOD_MASK_ANY);

    return 1;
}

void finalize_prism() {
    deconstruct_vector(groups);
    deconstruct_vector(managed_windows);

    finalize_xcb();
    finalize_ewmh();
}

void handle_xcb_events() {
    xcb_generic_event_t *xcb_event;
    unsigned int xcb_event_type;

    while ((xcb_event = xcb_poll_for_event(xcb_connection))) {
        xcb_event_type = xcb_event->response_type & ~0x80;

        if (xcb_events[xcb_event_type])
            xcb_events[xcb_event_type](xcb_event);

        free(xcb_event);
    }
}

void handle_signals(int signal) {
    if (signals[signal])
        signals[signal]();
}
