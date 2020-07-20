#include "connection.h"
#include "pointer.h"

xcb_query_pointer_cookie_t pointer_cookie;

void initialize_pointer() {
    pointer_cookie = xcb_query_pointer(xcb_connection, xcb_screen->root);
}

xcb_query_pointer_reply_t *get_pointer_coordinates() {
    xcb_query_pointer_reply_t *pointer = xcb_query_pointer_reply(xcb_connection,
        pointer_cookie, NULL);

    initialize_pointer();

    return pointer;
}
