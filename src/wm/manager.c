#include <unistd.h>

#include <xcb/xcb.h>

#include "group.h"
#include "manager.h"

#include "../handlers/ipc.h"
#include "../handlers/signal.h"
#include "../handlers/xcb.h"
#include "../ipc/socket.h"
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
    if (!initialize_xcb() || !initialize_ewmh() ||
        !initialize_socket(WINDOW_MANAGER_IPC)) {
        log_fatal("Unable to initialize window manager.");
        return 0;
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

    finalize_socket();
    finalize_ewmh();
    finalize_xcb();
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

void handle_ipc_input() {
    char *input = read_from_socket();

    ipc_command_t command = ipc_determine_command(input);
    char **args = ipc_split_input(input);

    char *response;
    if (ipc_commands[command])
        response = ipc_commands[command](args);

    if (response) {
        log_debug("responding: %s", response);
        write_to_socket(response);
    }

    unsigned int index = 0;
    while (args[index])
        free(args[index++]);

    free(input);
}

void handle_signals(int signal) {
    if (signals[signal])
        signals[signal]();
}
