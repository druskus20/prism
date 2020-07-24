#include <unistd.h>
#include <poll.h>

#include <xcb/xcb.h>

#include "group.h"
#include "handlers.h"
#include "manager.h"

#include "../globals.h"

#include "../xcb/connection.h"
#include "../xcb/ewmh.h"
#include "../xcb/pointer.h"

unsigned char window_manager_is_active = 1;
xcb_window_t focused_window; // REMOVE

vector_t *managed_windows = NULL;
vector_t *groups = NULL;

group_t *focused_group = NULL;

int window_manager() {
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
        XCB_GRAB_MODE_ASYNC, xcb_screen->root, XCB_NONE, 1, XCB_MOD_MASK_ANY);

    xcb_grab_button(xcb_connection, 0, xcb_screen->root, XCB_EVENT_MASK_BUTTON_PRESS |
            XCB_EVENT_MASK_BUTTON_RELEASE, XCB_GRAB_MODE_ASYNC,
            XCB_GRAB_MODE_ASYNC, xcb_screen->root, XCB_NONE, 3, XCB_MOD_MASK_ANY);

    struct pollfd file_descriptors[1] = {
        { xcb_file_descriptor, .events = POLLIN },
    };

    while (window_manager_is_active) {
        if (poll(file_descriptors, 1, -1)) {
            if (file_descriptors[0].revents & POLLIN)
                handle_xcb_events();

            // if (file_descriptors[1].revents & POLLIN)
        }
    }

    log_debug("Safely shutting down");

    deconstruct_vector(managed_windows);

    finalize_xcb();
    finalize_ewmh();

    return EXIT_SUCCESS;
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
